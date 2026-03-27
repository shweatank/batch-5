#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/gpio/consumer.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include "font8x8_basic.h"

#define DRIVER_NAME "ili9225"
#define CLASS_NAME  "ili"
#define DEVICE_NAME "ili9225_char"

#define SCREEN_WIDTH 176
#define SCREEN_HEIGHT 220

static dev_t dev_num;
static struct class *ili_class;
static struct cdev ili_cdev;
static struct ili9225 *g_lcd;

/* Cursor position */
static int cursor_x = 0;
static int cursor_y = 0;

struct ili9225 {
    struct spi_device *spi;
    struct gpio_desc *dc;
    struct gpio_desc *reset;
};

/* ---------------- SPI helpers ---------------- */
static int ili9225_write16(struct ili9225 *lcd, u16 value)
{
    u8 buf[2];
    buf[0] = value >> 8;
    buf[1] = value & 0xFF;
    return spi_write(lcd->spi, buf, 2);
}

static int ili9225_write_reg(struct ili9225 *lcd, u16 reg, u16 data)
{
    gpiod_set_value(lcd->dc, 0);
    ili9225_write16(lcd, reg);
    gpiod_set_value(lcd->dc, 1);
    return ili9225_write16(lcd, data);
}

static void ili9225_reset(struct ili9225 *lcd)
{
    gpiod_set_value(lcd->reset, 1);
    msleep(5);
    gpiod_set_value(lcd->reset, 0);
    msleep(20);
    gpiod_set_value(lcd->reset, 1);
    msleep(50);
}

static void ili9225_init(struct ili9225 *lcd)
{
    ili9225_reset(lcd);
    ili9225_write_reg(lcd, 0x0001, 0x011C);  // sets out put as top to bottom
    ili9225_write_reg(lcd, 0x0002, 0x0100);   // sets the line inversion
    ili9225_write_reg(lcd, 0x0003, 0x1030);    // GRAM entry mode
    ili9225_write_reg(lcd, 0x0008, 0x0808);   // control the frame frequency
    ili9225_write_reg(lcd, 0x000C, 0x0000);   // setting as a display control
    ili9225_write_reg(lcd, 0x000F, 0x0B01);   //power ups the lcd driver
    ili9225_write_reg(lcd, 0x0010, 0x0A00);   // internal voltages for pixels
    ili9225_write_reg(lcd, 0x0011, 0x1038); // ensures stable pixel driving voltage
    msleep(50);
    ili9225_write_reg(lcd, 0x0012, 0x1121);  //  driver output voltages
    ili9225_write_reg(lcd, 0x0013, 0x0063);  //ensures gate voltages for pixels
    ili9225_write_reg(lcd, 0x0014, 0x5A00);  // ensures uniform bright ness
    msleep(50);
    ili9225_write_reg(lcd, 0x0007, 0x1017);   // displays control on
    msleep(20);
}

// define drawing window
static void ili9225_fill(struct ili9225 *lcd, u16 color)
{
    int x, y;

    ili9225_write_reg(lcd, 0x0036, 175); //  horizontal end adress
    ili9225_write_reg(lcd, 0x0037, 0);  // horizontal syart adress
    ili9225_write_reg(lcd, 0x0038, 219); // verical end adress
    ili9225_write_reg(lcd, 0x0039, 0);   // verucal start adress
    ili9225_write_reg(lcd, 0x0020, 0);  // x_cursor position 
    ili9225_write_reg(lcd, 0x0021, 0);  // y_cursor position

    gpiod_set_value(lcd->dc, 0);    // if we set Dc=0 command mode
    ili9225_write16(lcd, 0x0022);   // GRAM data register tells next coming pixel data
    gpiod_set_value(lcd->dc, 1);     // switch data mode

    for (y = 0; y < SCREEN_HEIGHT; y++)
        for (x = 0; x < SCREEN_WIDTH; x++)
            ili9225_write16(lcd, color);      // setting the every pixel as awhite color
}

/* ---------------- Pixel / Character ---------------- */
static void drawPixel(int x, int y, uint16_t color)
{
    struct ili9225 *lcd = g_lcd;

    gpiod_set_value(lcd->dc, 0);   // command mode 
    ili9225_write16(lcd, 0x0020);  // configures where we have to write the pixels
    gpiod_set_value(lcd->dc, 1);  // data mode 
    ili9225_write16(lcd, x);      // writing the data

    gpiod_set_value(lcd->dc, 0);  
    ili9225_write16(lcd, 0x0021);
    gpiod_set_value(lcd->dc, 1);
    ili9225_write16(lcd, y);

    gpiod_set_value(lcd->dc, 0);
    ili9225_write16(lcd, 0x0022);
    gpiod_set_value(lcd->dc, 1);
    ili9225_write16(lcd, color);
}

static void drawChar(int x, int y, char c, uint16_t color)
{
    if(c < 32 || c > 127) return;                // if other than charcter then return the function otherwise continuw 
    const uint8_t *bitmap = font8x8[c - 32];

    for(int row = 0; row < 8; row++)
    {
        for(int col = 0; col < 8; col++)
        {
            if(bitmap[row] & (1 << (7 - col)))
            {
                drawPixel(x + col*2, y + row*2, color);
                drawPixel(x + col*2+1, y + row*2, color);
                drawPixel(x + col*2, y + row*2+1, color);
                drawPixel(x + col*2+1, y + row*2+1, color);
            }
        }
    }
}

static void drawString(int *x, int *y, const char *str, uint16_t color)
{
    int start_x = *x;
    for(int i=0; str[i]; i++)
    {
        if(str[i] == '\n') {
            *x = start_x;            // this part is explain if character is newline the cursor sets new line 
            *y += 16;
            if(*y > SCREEN_HEIGHT-16) *y = 0;
            continue;
        }

        drawChar(*x, *y, str[i], color);       // if not print the character on lcd display
        *x += 16;

        if(*x > SCREEN_WIDTH - 16) {
            *x = start_x;
            *y += 16;
            if(*y > SCREEN_HEIGHT-16) *y = 0;
        }
    }
}

/* ---------------- Char Device Write ---------------- */
static ssize_t ili_write(struct file *file,
                         const char __user *buf,
                         size_t len,
                         loff_t *off)
{
    char kbuf[128];

    if (len > 127)
        len = 127;

    if (copy_from_user(kbuf, buf, len))
        return -EFAULT;

    kbuf[len] = '\0';

    /* Clear screen and reset cursor */
    ili9225_fill(g_lcd, 0xFFFF);  // white background
    cursor_x = 0;
    cursor_y = 20;

    /* Draw the new string */
    drawString(&cursor_x, &cursor_y, kbuf, 0x0000); // black text

    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = ili_write,
};

// SPI Probe / Remove   function

static int ili9225_probe(struct spi_device *spi)
{
    struct ili9225 *lcd;

    lcd = devm_kzalloc(&spi->dev, sizeof(*lcd), GFP_KERNEL);
    if (!lcd) return -ENOMEM;    //kernel allcates the memory using kzalloc api

    lcd->spi = spi;
    spi_set_drvdata(spi, lcd);

    lcd->dc = devm_gpiod_get(&spi->dev, "dc", GPIOD_OUT_LOW);
    if (IS_ERR(lcd->dc)) return PTR_ERR(lcd->dc);

    lcd->reset = devm_gpiod_get(&spi->dev, "reset", GPIOD_OUT_HIGH);
    if (IS_ERR(lcd->reset)) return PTR_ERR(lcd->reset);

    spi->mode = SPI_MODE_0;  // CPOL =0,CPHA = 0
    spi->bits_per_word = 8;  // we have to send 8 bits at a time
    spi_setup(spi);

    ili9225_init(lcd);           // lcd init function
    ili9225_fill(lcd, 0xFFFF);  // setting height and width of lcd display 

    g_lcd = lcd;

    alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    cdev_init(&ili_cdev, &fops);
    cdev_add(&ili_cdev, dev_num, 1);

    ili_class = class_create(CLASS_NAME);
    device_create(ili_class, NULL, dev_num, NULL, DEVICE_NAME);

    dev_info(&spi->dev, "ILI9225 initialized\n");
    return 0;
}

static void ili9225_remove(struct spi_device *spi)
{
    device_destroy(ili_class, dev_num);
    class_destroy(ili_class);
    cdev_del(&ili_cdev);
    unregister_chrdev_region(dev_num, 1);

    dev_info(&spi->dev, "ILI9225 removed\n");
}

/* ---------------- Device Tree ---------------- */
static const struct of_device_id ili9225_dt_ids[] = {
    { .compatible = "ilitek,ili9225" },
    { }
};  // for matching the device tree configuration
MODULE_DEVICE_TABLE(of, ili9225_dt_ids);

/* ---------------- SPI Driver ---------------- */
static struct spi_driver ili9225_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = ili9225_dt_ids,
    },
    .probe  = ili9225_probe,
    .remove = ili9225_remove,
};

module_spi_driver(ili9225_driver);    //load the driver when inserted using insmod

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TEAM 4");
MODULE_DESCRIPTION("ILI9225 SPI LCD Driver with text wrapping");
