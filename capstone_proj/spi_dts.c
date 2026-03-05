#include <linux/module.h>        // Required for loadable kernel module
#include <linux/spi/spi.h>       // SPI subsystem APIs
#include <linux/gpio/consumer.h> // Descriptor based GPIO API
#include <linux/delay.h>         // For msleep()
#include <linux/of.h>            // Device Tree support
#include <linux/fs.h>            // File system support
#include <linux/cdev.h>          // Character device registration
#include <linux/device.h>        // Device class creation
#include <linux/uaccess.h>       // copy_to_user / copy_from_user
#include "font8x8_basic.h"       // 8x8 ASCII font table

#define DRIVER_NAME "ili9225"        // SPI driver name
#define CLASS_NAME  "ili"            // Device class name
#define DEVICE_NAME "ili9225_char"   // /dev device name

#define SCREEN_WIDTH 176             // LCD width
#define SCREEN_HEIGHT 220            // LCD height

static dev_t dev_num;                // Device number (major+minor)
static struct class *ili_class;     // Device class pointer
static struct cdev ili_cdev;        // Character device structure
static struct ili9225 *g_lcd;       // Global LCD pointer

/* Cursor position */
static int cursor_x = 0;            // X position for text
static int cursor_y = 0;            // Y position for text

/* LCD device structure */
struct ili9225 {                      
    struct spi_device *spi;         //used to communicate with display via SPI bus
    struct gpio_desc *dc;          // register select  rs=0 ->command rs=1 ->data //gpio controller,flags,consumer name
    struct gpio_desc *reset;        // Reset the display
};

/* ---------------- SPI helpers ---------------- */

/* Send 16-bit value over SPI */
static int ili9225_write16(struct ili9225 *lcd, u16 value)
{
    u8 buf[2];                      // 2-byte buffer
    buf[0] = value >> 8;            // High byte
    buf[1] = value & 0xFF;          // Low byte
    return spi_write(lcd->spi, buf, 2);  // Send via SPI
}

/* Write register and data to LCD */
static int ili9225_write_reg(struct ili9225 *lcd, u16 reg, u16 data)
{
    gpiod_set_value(lcd->dc, 0);    // DC=0 → Command
    ili9225_write16(lcd, reg);      // Send register address
    gpiod_set_value(lcd->dc, 1);    // DC=1 → Data
    return ili9225_write16(lcd, data); // Send register data
}

/* Hardware reset sequence */
static void ili9225_reset(struct ili9225 *lcd)
{
    gpiod_set_value(lcd->reset, 1); // Set reset high
    msleep(5);                     // Delay 5ms
    gpiod_set_value(lcd->reset, 0);// Set reset low
    msleep(20);                    // Delay 20ms
    gpiod_set_value(lcd->reset, 1);// Set reset high again
    msleep(50);                    // final stabilization delay
}

/* LCD initialization sequence */
static void ili9225_init(struct ili9225 *lcd)
{
    ili9225_reset(lcd);             // Reseting the  LCD to clear privious logics(if any)

    ili9225_write_reg(lcd, 0x0001, 0x011C); // scan the screen from top to bottom 
    ili9225_write_reg(lcd, 0x0002, 0x0100); // allows flipping of voltage between pixels
    ili9225_write_reg(lcd, 0x0003, 0x1030); // Entry Mode
    ili9225_write_reg(lcd, 0x0008, 0x0808);  //Controls idle time between frames
    ili9225_write_reg(lcd, 0x000C, 0x0000); // selects internal clock
    ili9225_write_reg(lcd, 0x000F, 0x0B01); //control clock frequency
    ili9225_write_reg(lcd, 0x0010, 0x0A00); // sets internal voltage
    ili9225_write_reg(lcd, 0x0011, 0x1038);  //Enables internal circuits and starts charge pump
    msleep(50);                     // Power stabilization delay
    ili9225_write_reg(lcd, 0x0012, 0x1121); //Sets reference voltage for pixels brightness
    ili9225_write_reg(lcd, 0x0013, 0x0063);   //Controls voltage switching
    ili9225_write_reg(lcd, 0x0014, 0x5A00);  //Fine tuning
    msleep(50);                     // Delay
    ili9225_write_reg(lcd, 0x0007, 0x1017); // Display ON
    msleep(20);                     // Final delay
}

/* Fill entire screen with one color */
static void ili9225_fill(struct ili9225 *lcd, u16 color)
{
    int x, y;                       // Loop variables

    ili9225_write_reg(lcd, 0x0036, 175); // Horizontal End Address
    ili9225_write_reg(lcd, 0x0037, 0);   // Horizontal Start Address
    ili9225_write_reg(lcd, 0x0038, 219); // Vertical End Address
    ili9225_write_reg(lcd, 0x0039, 0);   // Vertical Start Address
    ili9225_write_reg(lcd, 0x0020, 0);   // Set X address
    ili9225_write_reg(lcd, 0x0021, 0);   // Set Y address

    gpiod_set_value(lcd->dc, 0);         // Command mode
    ili9225_write16(lcd, 0x0022);        // RAM write command
    gpiod_set_value(lcd->dc, 1);         // Data mode

    for (y = 0; y < SCREEN_HEIGHT; y++)  // For each row
        for (x = 0; x < SCREEN_WIDTH; x++) // For each column
            ili9225_write16(lcd, color);   // Write pixel color
}

/* Draw single pixel */
static void drawPixel(int x, int y, uint16_t color)
{
    struct ili9225 *lcd = g_lcd;   // Get LCD pointer

    gpiod_set_value(lcd->dc, 0);   // Command mode
    ili9225_write16(lcd, 0x0020);  // Set X address command
    gpiod_set_value(lcd->dc, 1);   // Data mode
    ili9225_write16(lcd, x);       // Send X coordinate

    gpiod_set_value(lcd->dc, 0);   // Command mode
    ili9225_write16(lcd, 0x0021);  // Set Y address command
    gpiod_set_value(lcd->dc, 1);   // Data mode
    ili9225_write16(lcd, y);       // Send Y coordinate

    gpiod_set_value(lcd->dc, 0);   // Command mode
    ili9225_write16(lcd, 0x0022);  // RAM write command
    gpiod_set_value(lcd->dc, 1);   // Data mode
    ili9225_write16(lcd, color);   // Send pixel color
}

/* Draw one character (scaled 2x) */
static void drawChar(int x, int y, char c, uint16_t color)
{
    if(c < 32 || c > 127) return;  // Ignore non-printable ASCII
    const uint8_t *bitmap = font8x8[c - 32]; // Get font bitmap

    for(int row = 0; row < 8; row++)           // For each row
    {
        for(int col = 0; col < 8; col++)       // For each column
        {
            if(bitmap[row] & (1 << (7 - col))) // If pixel is set
            {
                drawPixel(x + col*2, y + row*2, color);
                drawPixel(x + col*2+1, y + row*2, color);
                drawPixel(x + col*2, y + row*2+1, color);
                drawPixel(x + col*2+1, y + row*2+1, color);
            }
        }
    }
}

/* Draw string with wrapping */
static void drawString(int *x, int *y, const char *str, uint16_t color)
{
    int start_x = *x;              // Store starting X

    for(int i=0; str[i]; i++)      // Loop through string
    {
        if(str[i] == '\n') {       // If newline
            *x = start_x;          // Reset X
            *y += 16;              // Move down
            if(*y > SCREEN_HEIGHT-16) *y = 0; // Wrap to top
            continue;
        }

        drawChar(*x, *y, str[i], color); // Draw character
        *x += 16;                        // Move right

        if(*x > SCREEN_WIDTH - 16) {     // If exceeds width
            *x = start_x;
            *y += 16;
            if(*y > SCREEN_HEIGHT-16) *y = 0;
        }
    }
}

/* Write from user space */
static ssize_t ili_write(struct file *file,
                         const char __user *buf,
                         size_t len,
                         loff_t *off)
{
    char kbuf[128];                // Kernel buffer

    if (len > 127)                 // Limit length
        len = 127;

    if (copy_from_user(kbuf, buf, len)) // Copy from user
        return -EFAULT;

    kbuf[len] = '\0';              // Null terminate

    ili9225_fill(g_lcd, 0xFFFF);   // Clear screen (white)
    cursor_x = 0;                  // Reset cursor X
    cursor_y = 0;                  // Reset cursor Y

    drawString(&cursor_x, &cursor_y, kbuf, 0x0000); // Draw text

    return len;                    // Return bytes written
}

/* File operations */
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = ili_write,
};

/* SPI probe function */
static int ili9225_probe(struct spi_device *spi)
{
    struct ili9225 *lcd;

    lcd = devm_kzalloc(&spi->dev, sizeof(*lcd), GFP_KERNEL); // Allocate memory
    if (!lcd) return -ENOMEM;

    lcd->spi = spi;                // Store SPI device
    spi_set_drvdata(spi, lcd);     // Attach private data

    lcd->dc = devm_gpiod_get(&spi->dev, "dc", GPIOD_OUT_LOW); // Get DC GPIO
    if (IS_ERR(lcd->dc)) return PTR_ERR(lcd->dc);

    lcd->reset = devm_gpiod_get(&spi->dev, "reset", GPIOD_OUT_HIGH); // Get Reset GPIO
    if (IS_ERR(lcd->reset)) return PTR_ERR(lcd->reset);

    spi->mode = SPI_MODE_0;        // SPI mode 0 cpol=0,cpha=0
    spi->bits_per_word = 8;        // 8-bit transfers
    spi_setup(spi);                // Apply SPI settings

    ili9225_init(lcd);             // Initialize LCD
    ili9225_fill(lcd, 0xFFFF);     // Clear screen

    g_lcd = lcd;                   // Save global pointer

    alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME); // Allocate device number
    cdev_init(&ili_cdev, &fops);   // Initialize cdev
    cdev_add(&ili_cdev, dev_num, 1); // Add to kernel

    ili_class = class_create(CLASS_NAME); // Create device class
    device_create(ili_class, NULL, dev_num, NULL, DEVICE_NAME); // Create /dev node

    dev_info(&spi->dev, "ILI9225 initialized\n");
    return 0;
}

/* Remove function */
static void ili9225_remove(struct spi_device *spi)
{
    device_destroy(ili_class, dev_num); // Remove device node
    class_destroy(ili_class);           // Destroy class
    cdev_del(&ili_cdev);                // Delete cdev
    unregister_chrdev_region(dev_num, 1); // Free device number

    dev_info(&spi->dev, "ILI9225 removed\n");
}

/* Device Tree match table */
static const struct of_device_id ili9225_dt_ids[] = {
    { .compatible = "ilitek,ili9225" }, // Match DT compatible string
    { }
};
MODULE_DEVICE_TABLE(of, ili9225_dt_ids);

/* SPI driver structure */
static struct spi_driver ili9225_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = ili9225_dt_ids,
    },
    .probe  = ili9225_probe,
    .remove = ili9225_remove,
};

module_spi_driver(ili9225_driver); // Register SPI driver

MODULE_LICENSE("GPL");          
MODULE_AUTHOR("Your Name");        
MODULE_DESCRIPTION("ILI9225 SPI LCD Driver with text wrapping"); 
