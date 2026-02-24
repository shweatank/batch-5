// ili9225_spi_driver.c
#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/device.h>

#define DEVICE_NAME "ili9225"
#define DC_PIN 25
#define RST_PIN 24

static struct spi_device *ili_spi;
static dev_t dev_num;
static struct cdev ili_cdev;
static struct class *ili_class;

/* Simple font for A, B, C (expand as needed) */
static const uint8_t font8x8[3][8] = {
    {0x18,0x3C,0x24,0x66,0x7E,0x66,0x66,0x00}, // A
    {0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00}, // B
    {0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00}, // C
};

/* ---------------- SPI WRITE ---------------- */

static void write16(uint16_t value)
{
    uint8_t buf[2];
    buf[0] = value >> 8;
    buf[1] = value & 0xFF;
    spi_write(ili_spi, buf, 2);
}

static void write_reg(uint16_t reg, uint16_t data)
{
    gpio_set_value(DC_PIN, 0); // command
    write16(reg);
    gpio_set_value(DC_PIN, 1); // data
    write16(data);
}

/* ---------------- LCD INIT ---------------- */

static void lcd_reset(void)
{
    gpio_set_value(RST_PIN, 1);
    mdelay(5);
    gpio_set_value(RST_PIN, 0);
    mdelay(20);
    gpio_set_value(RST_PIN, 1);
    mdelay(50);
}

static void lcd_init_hw(void)
{
    lcd_reset();

    write_reg(0x0001, 0x011C);
    write_reg(0x0002, 0x0100);
    write_reg(0x0003, 0x1030);
    write_reg(0x0008, 0x0808);
    write_reg(0x000C, 0x0000);
    write_reg(0x000F, 0x0B01);

    write_reg(0x0010, 0x0A00);
    write_reg(0x0011, 0x1038);
    mdelay(50);
    write_reg(0x0012, 0x1121);
    write_reg(0x0013, 0x0063);
    write_reg(0x0014, 0x5A00);
    mdelay(50);

    write_reg(0x0007, 0x1017);
    mdelay(20);
}

/* ---------------- PIXEL & CHAR ---------------- */

static void draw_pixel(int x, int y, uint16_t color)
{
    gpio_set_value(DC_PIN, 0);
    write16(0x0020);
    gpio_set_value(DC_PIN, 1);
    write16(x);

    gpio_set_value(DC_PIN, 0);
    write16(0x0021);
    gpio_set_value(DC_PIN, 1);
    write16(y);

    gpio_set_value(DC_PIN, 0);
    write16(0x0022);
    gpio_set_value(DC_PIN, 1);
    write16(color);
}

static void draw_char(int x, int y, char c, uint16_t color)
{
    if (c < 'A' || c > 'C')
        return;

    const uint8_t *bitmap = font8x8[c - 'A'];
    int row, col;

    for (row = 0; row < 8; row++)
    {
        for (col = 0; col < 8; col++)
        {
            if (bitmap[row] & (1 << (7 - col)))
            {
                draw_pixel(x + col*2, y + row*2, color);
                draw_pixel(x + col*2+1, y + row*2, color);
                draw_pixel(x + col*2, y + row*2+1, color);
                draw_pixel(x + col*2+1, y + row*2+1, color);
            }
        }
    }
}

/* ---------------- FILE OPS ---------------- */

static ssize_t ili_write(struct file *file,
                         const char __user *buf,
                         size_t len,
                         loff_t *offset)
{
    char kbuf[32];
    int i;

    if (len > 31)
        len = 31;

    if (copy_from_user(kbuf, buf, len))
        return -EFAULT;

    for (i = 0; i < len; i++)
        draw_char(i*16, 0, kbuf[i], 0xF800); // draw red text

    return len;
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = ili_write,
};

/* ---------------- SPI DRIVER ---------------- */

static int ili9225_probe(struct spi_device *spi)
{
    int ret;

    pr_info("ILI9225 SPI device probed\n");
    ili_spi = spi;

    gpio_request_one(DC_PIN, GPIOF_OUT_INIT_LOW, "ili9225_dc");
    gpio_request_one(RST_PIN, GPIOF_OUT_INIT_HIGH, "ili9225_rst");

    lcd_init_hw();

    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret < 0)
        return ret;

    cdev_init(&ili_cdev, &fops);
    cdev_add(&ili_cdev, dev_num, 1);

    ili_class = class_create(DEVICE_NAME);
    if (IS_ERR(ili_class))
        return PTR_ERR(ili_class);

    device_create(ili_class, NULL, dev_num, NULL, DEVICE_NAME);

    return 0;
}

static void ili9225_remove(struct spi_device *spi)
{
    device_destroy(ili_class, dev_num);
    class_destroy(ili_class);
    cdev_del(&ili_cdev);
    unregister_chrdev_region(dev_num, 1);

    gpio_free(DC_PIN);
    gpio_free(RST_PIN);

    pr_info("ILI9225 SPI device removed\n");
}

static const struct of_device_id ili9225_dt_ids[] = {
    { .compatible = "spi,ili9225", },
    {}
};
MODULE_DEVICE_TABLE(of, ili9225_dt_ids);

static struct spi_driver ili9225_driver = {
    .driver = {
        .name = "ili9225",
        .of_match_table = ili9225_dt_ids,
    },
    .probe = ili9225_probe,
    .remove = ili9225_remove,
};

module_spi_driver(ili9225_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("OpenAI ChatGPT");
MODULE_DESCRIPTION("SPI driver for ILI9225 LCD with character display");

