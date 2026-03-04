#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/gpio/consumer.h> //descriptor based gpio API
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/fs.h>
#include <linux/cdev.h>//char dev registration
#include <linux/device.h>
#include <linux/uaccess.h>// user to kernel space 
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
    struct spi_device *spi;//used to communicate with display via SPI bus
    struct gpio_desc *dc;// register select  rs=0 ->command rs=1 ->data //gpio controller,flags,consumer
    struct gpio_desc *reset;//reset the display
};

/* ---------------- SPI helpers ---------------- */

/*
	Storing data into buffer and sending to a pointer that represents hardware
*/

static int ili9225_write16(struct ili9225 *lcd, u16 value)
{
    u8 buf[2];
    buf[0] = value >> 8;
    buf[1] = value & 0xFF;
    return spi_write(lcd->spi, buf, 2);
}

static int ili9225_write_reg(struct ili9225 *lcd, u16 reg, u16 data)
{

/*---To send command ---*/
    gpiod_set_value(lcd->dc, 0);
    ili9225_write16(lcd, reg);
    /*---To send data---*/
    gpiod_set_value(lcd->dc, 1);
    return ili9225_write16(lcd, data);
}

static void ili9225_reset(struct ili9225 *lcd)
{
    gpiod_set_value(lcd->reset, 1); //pull reset pin high
    msleep(5);//wait for 5ms
    gpiod_set_value(lcd->reset, 0);//pull low (trigger reset)
    msleep(20);                    //wait for 5ms 
    gpiod_set_value(lcd->reset, 1);//again pull high(Release reset pin)
    msleep(50);                    //final stabilization delay
}


static void ili9225_display_init(struct ili9225_display *lcd)
{
    ili9225_display_reset(lcd);                           //Resetting the LCD to clear previous logic

    ili9225_display_write_reg(lcd, 0x0001, 0x011C);       //Scan the screen from top to bottom
    ili9225_display_write_reg(lcd, 0x0002, 0x0100);       //Allows flipping of voltage between pixels

    // LANDSCAPE ENTRY MODE
    ili9225_display_write_reg(lcd, 0x0003, 0x1038);       //Tells address counter to increment horizantally(Landscape)

    ili9225_display_write_reg(lcd, 0x0008, 0x0808);       //Controls idle time between frames
    ili9225_display_write_reg(lcd, 0x000C, 0x0000);       //Selects internal clock
    ili9225_display_write_reg(lcd, 0x000F, 0x0B01);       //Controls clock frequency

    ili9225_display_write_reg(lcd, 0x0010, 0x0A00);       //Sets internal voltage
    ili9225_display_write_reg(lcd, 0x0011, 0x1038);       //Enables internal circuits and starts charge pump
    msleep(50);                                           //wait for 50ms to get voltage stability

    ili9225_display_write_reg(lcd, 0x0012, 0x1121);       //Sets reference voltage for pixels brightness
    ili9225_display_write_reg(lcd, 0x0013, 0x0063);       //Controls voltage switching
    ili9225_display_write_reg(lcd, 0x0014, 0x5A00);       //Fine tuning
    msleep(50);                                           //Delay for stabilizing voltage

    ili9225_display_write_reg(lcd, 0x0007, 0x1017);       //Enables Internal logic, Display output
    msleep(20);                                           //Delay to ensure stable start
}

static void ili9225_display_fill(struct ili9225_display *lcd, u16 color)
{
    int x, y;

    ili9225_display_write_reg(lcd, 0x0036, LCD_HEIGHT - 1);   //Sets vertical bottom limit
    ili9225_display_write_reg(lcd, 0x0037, 0);                //sets vertical top limit
    ili9225_display_write_reg(lcd, 0x0038, LCD_WIDTH - 1);    //sets horizontal end
    ili9225_display_write_reg(lcd, 0x0039, 0);                //sets horizantal start

    ili9225_display_write_reg(lcd, 0x0020, 0);                //Sets coloumn pointer to 0
    ili9225_display_write_reg(lcd, 0x0021, 0);               //Sets row pointer to 0

    gpiod_set_value(lcd->dc, 0);                             //Setting low (To send ommand)
    ili9225_display_write16(lcd, 0x0022);                    //This is GRAM data register
    gpiod_set_value(lcd->dc, 1);                            //Setting high to send data

    for (y = 0; y < LCD_HEIGHT; y++)
        for (x = 0; x < LCD_WIDTH; x++)
            ili9225_display_write16(lcd, color);             //Filling screen with (white) background
}



/* ---------------- Pixel / Character ---------------- */
static void drawPixel(int x, int y, uint16_t color)
{
    struct ili9225 *lcd = g_lcd;

     gpiod_set_value(lcd->dc, 0);
    ili9225_display_write16(lcd, 0x0020);                //Setting row position
    gpiod_set_value(lcd->rs, 1);
    ili9225_display_write16(lcd, y_rotated); // Use y_rotated for Gram Address Set

    gpiod_set_value(lcd->dc, 0);
    ili9225_display_write16(lcd, 0x0021);                //Setting coloumn position
    gpiod_set_value(lcd->dc, 1);
    ili9225_display_write16(lcd, x_rotated); // Use x_rotated for Gram Address Set
/////////////Setting colour//////////////////////////////
    gpiod_set_value(lcd->dc, 0);
    ili9225_display_write16(lcd, 0x0022);
    gpiod_set_value(lcd->dc, 1);
    ili9225_display_write16(lcd, color);
}
}

static void drawChar(int x, int y, char c, uint16_t color)
{
    if(c < 32 || c > 127) return;//allow printable characters
    const uint8_t *bitmap = font8x8[c - 32];//storing the charecter

    for(int row = 0; row < 8; row++)
    {
        for(int col = 0; col < 8; col++)
        {
            if(bitmap[row] & (1 << (7 - col)))//1->draw pixel,0->skip pixel
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
            *x = start_x;
            *y += 16;
            if(*y > SCREEN_HEIGHT-16) *y = 0;
            continue;
        }

        drawChar(*x, *y, str[i], color);
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
    cursor_y = 0;

    /* Draw the new string */
    drawString(&cursor_x, &cursor_y, kbuf, 0x0000); // black text

    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = ili_write,
};

/* ---------------- SPI Probe / Remove ---------------- */
static int ili9225_probe(struct spi_device *spi)
{
    struct ili9225 *lcd;

    lcd = devm_kzalloc(&spi->dev, sizeof(*lcd), GFP_KERNEL);//allocate memory for struct ili9225 and automatically freed when driver is removed
    if (!lcd)
    if (!lcd) return -ENOMEM;

    lcd->spi = spi;//store the SPI device pointer to our structure (in driver)
    spi_set_drvdata(spi, lcd);//store lcd structure inside SPI device (in kernel)


    lcd->dc = devm_gpiod_get(&spi->dev, "dc", GPIOD_OUT_LOW);// get GPIO pin from DT and set to low
    if (IS_ERR(lcd->dc)) return PTR_ERR(lcd->dc);


    lcd->reset = devm_gpiod_get(&spi->dev, "reset", GPIOD_OUT_HIGH);//get GPIO pin from DT and set to high

    if (IS_ERR(lcd->reset)) return PTR_ERR(lcd->reset);


    spi->mode = SPI_MODE_0;//cpol=0,cpha=0
    spi->bits_per_word = 8;//fill screen with white color
    spi_setup(spi);//applying spi configuration to hardware controller

    ili9225_init(lcd);//initializing display
    ili9225_fill(lcd, 0xFFFF);//fill screen with white colour

    g_lcd = lcd;//savre pointer to globally

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
/*seperates hardware details from drivercode and makes driver portable */
static const struct of_device_id ili9225_dt_ids[] = {
    { .compatible = "ilitek,ili9225" },
    { }
};
MODULE_DEVICE_TABLE(of, ili9225_dt_ids);

/* ---------------- SPI Driver ---------------- */
static struct spi_driver ili9225_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = ili9225_dt_ids,//used to match compatible string from DT
    },
    .probe  = ili9225_probe,//called when matching spi device is found
    .remove = ili9225_remove,//called when device is removed
};

module_spi_driver(ili9225_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("ILI9225 SPI LCD Driver with text wrapping");
