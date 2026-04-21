#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/gpio/consumer.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include "image_file.h"

#define DRIVER_NAME "ili9225"
#define CLASS_NAME  "ili"
#define DEVICE_NAME "ili9225_char"

#define SCREEN_WIDTH 176
#define SCREEN_HEIGHT 220
#define FRAME_SIZE (SCREEN_WIDTH*SCREEN_HEIGHT*2)

unsigned short int *p[]={image1,image2,image3,image4,image5};

static dev_t dev_num;
static struct class *ili_class;
static struct cdev ili_cdev;

struct ili9225 {
    struct spi_device *spi;
    struct gpio_desc *dc;
    struct gpio_desc *reset;
};

static struct ili9225 *g_lcd;

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

    ili9225_write_reg(lcd, 0x0001, 0x011C);
    ili9225_write_reg(lcd, 0x0002, 0x0100);
    ili9225_write_reg(lcd, 0x0003, 0x1030);
    ili9225_write_reg(lcd, 0x0008, 0x0808);
    ili9225_write_reg(lcd, 0x000C, 0x0000);
    ili9225_write_reg(lcd, 0x000F, 0x0B01);
    ili9225_write_reg(lcd, 0x0010, 0x0A00);
    ili9225_write_reg(lcd, 0x0011, 0x1038);

    msleep(50);

    ili9225_write_reg(lcd, 0x0012, 0x1121);
    ili9225_write_reg(lcd, 0x0013, 0x0063);
    ili9225_write_reg(lcd, 0x0014, 0x5A00);

    msleep(50);

    ili9225_write_reg(lcd, 0x0007, 0x1017);

    msleep(20);
}

static void ili9225_set_window(struct ili9225 *lcd)
{
    ili9225_write_reg(lcd, 0x0036, SCREEN_WIDTH-1);
    ili9225_write_reg(lcd, 0x0037, 0);
    ili9225_write_reg(lcd, 0x0038, SCREEN_HEIGHT-1);
    ili9225_write_reg(lcd, 0x0039, 0);

    ili9225_write_reg(lcd, 0x0020, 0);
    ili9225_write_reg(lcd, 0x0021, 0);
}

static void ili9225_draw_image(struct ili9225 *lcd, u16 *buf)
{
    int i,j=0;

    ili9225_set_window(lcd);

    gpiod_set_value(lcd->dc, 0);
    ili9225_write16(lcd, 0x0022);

    gpiod_set_value(lcd->dc, 1);

    for(j=0;j<5;j++)
    {

    for(i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++)
        ili9225_write16(lcd, p[j][i]);

    msleep(1000);

    }
}

static ssize_t ili_write(struct file *file,
                         const char __user *buf,
                         size_t len,
                         loff_t *off)
{
    u16 *frame;

    if(len != FRAME_SIZE)
        return -EINVAL;

    frame = kmalloc(FRAME_SIZE, GFP_KERNEL);
    if(!frame)
        return -ENOMEM;

    if(copy_from_user(frame, buf, FRAME_SIZE))
    {
        kfree(frame);
        return -EFAULT;
    }

    ili9225_draw_image(g_lcd, frame);

    kfree(frame);

    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = ili_write,
};

static int ili9225_probe(struct spi_device *spi)
{
    struct ili9225 *lcd;

    lcd = devm_kzalloc(&spi->dev, sizeof(*lcd), GFP_KERNEL);
    if(!lcd)
        return -ENOMEM;

    lcd->spi = spi;

    lcd->dc = devm_gpiod_get(&spi->dev, "dc", GPIOD_OUT_LOW);
    lcd->reset = devm_gpiod_get(&spi->dev, "reset", GPIOD_OUT_HIGH);

    spi->mode = SPI_MODE_0;
    spi->bits_per_word = 8;

    spi_setup(spi);

    ili9225_init(lcd);

    g_lcd = lcd;

    alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);

    cdev_init(&ili_cdev, &fops);
    cdev_add(&ili_cdev, dev_num, 1);

    ili_class = class_create(CLASS_NAME);

    device_create(ili_class, NULL, dev_num, NULL, DEVICE_NAME);

    dev_info(&spi->dev, "ILI9225 image driver loaded\n");

    return 0;
}

static void ili9225_remove(struct spi_device *spi)
{
    device_destroy(ili_class, dev_num);

    class_destroy(ili_class);

    cdev_del(&ili_cdev);

    unregister_chrdev_region(dev_num,1);
}

static const struct of_device_id ili9225_dt_ids[] = {
    { .compatible = "ilitek,ili9225" },
    { }
};

MODULE_DEVICE_TABLE(of, ili9225_dt_ids);

static struct spi_driver ili9225_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = ili9225_dt_ids,
    },
    .probe = ili9225_probe,
    .remove = ili9225_remove,
};

module_spi_driver(ili9225_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Driver Example");
MODULE_DESCRIPTION("ILI9225 Image Display Driver");

