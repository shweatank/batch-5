// SPDX-License-Identifier: GPL
#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio/consumer.h>
#include <linux/delay.h>

#define DEVICE_NAME "ili9225"

static dev_t devno;
static struct cdev ili_cdev;
static struct class *ili_class;

struct ili_data {
    struct spi_device *spi;
    struct gpio_desc *dc;
    struct gpio_desc *reset;
};

static struct ili_data *lcd;

/* ---------- SPI WRITE ---------- */

static void ili_write(u8 dc, u8 *buf, int len)
{
    gpiod_set_value(lcd->dc, dc);
    spi_write(lcd->spi, buf, len);
}

static void ili_cmd(u16 cmd)
{
    u8 d[2] = { cmd >> 8, cmd & 0xFF };
    ili_write(0, d, 2);
}

static void ili_data(u16 data)
{
    u8 d[2] = { data >> 8, data & 0xFF };
    ili_write(1, d, 2);
}

/* ---------- RESET ---------- */

static void ili_reset(void)
{
    gpiod_set_value(lcd->reset, 1);
    msleep(10);

    gpiod_set_value(lcd->reset, 0);
    msleep(50);

    gpiod_set_value(lcd->reset, 1);
    msleep(150);
}

/* ---------- INIT ---------- */
static void ili_init_display(void)
{
    ili_reset();

    ili_cmd(0x01); ili_data(0x011C);
    ili_cmd(0x02); ili_data(0x0100);
    ili_cmd(0x03); ili_data(0x1030);
    ili_cmd(0x08); ili_data(0x0808);
    ili_cmd(0x0C); ili_data(0x0000);
    ili_cmd(0x0F); ili_data(0x0D01);

    ili_cmd(0x20); ili_data(0x0000);
    ili_cmd(0x21); ili_data(0x0000);

    /* Power ON sequence */
    ili_cmd(0x10); ili_data(0x0000);
    ili_cmd(0x11); ili_data(0x0000);
    ili_cmd(0x12); ili_data(0x0000);
    ili_cmd(0x13); ili_data(0x0000);
    msleep(40);

    ili_cmd(0x10); ili_data(0x17B0);
    ili_cmd(0x11); ili_data(0x0037);
    msleep(40);

    ili_cmd(0x12); ili_data(0x0138);
    msleep(40);

    ili_cmd(0x13); ili_data(0x1700);
    ili_cmd(0x29); ili_data(0x000D);
    msleep(40);

    ili_cmd(0x07); ili_data(0x1017);

    pr_info("ILI9225 Initialized\n");
}

/* ---------- DRAW ---------- */

static void draw_pixel(u16 x, u16 y, u16 color)
{
    ili_cmd(0x20); ili_data(x);
    ili_cmd(0x21); ili_data(y);
    ili_cmd(0x22);
    ili_data(color);
}

/* ---------- CHAR DRIVER ---------- */

static ssize_t ili_write_user(struct file *f,
                              const char __user *buf,
                              size_t len,
                              loff_t *off)
{
    draw_pixel(50, 50, 0xF800);   // red pixel test
    return len;
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = ili_write_user,
};

/* ---------- PROBE ---------- */

static int ili_probe(struct spi_device *spi)
{
    lcd = devm_kzalloc(&spi->dev, sizeof(*lcd), GFP_KERNEL);

    lcd->spi = spi;
    lcd->dc = devm_gpiod_get(&spi->dev, "dc", GPIOD_OUT_LOW);
    lcd->reset = devm_gpiod_get(&spi->dev, "reset", GPIOD_OUT_HIGH);

    spi->mode = SPI_MODE_0;
    spi->bits_per_word = 8;
    spi->max_speed_hz = 4000000;
    spi_setup(spi);

    ili_init_display();

    /* Char device */
    alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME);
    cdev_init(&ili_cdev, &fops);
    cdev_add(&ili_cdev, devno, 1);

    ili_class = class_create(DEVICE_NAME);
device_create(ili_class, NULL, devno, NULL, DEVICE_NAME);
/* Fill screen RED */
int x,y;
for (x = 0; x < 176; x++)
for (y = 0; y < 220; y++) {
    ili_cmd(0x20); ili_data(x);
    ili_cmd(0x21); ili_data(y);
    ili_cmd(0x22);
    ili_data(0xF800);
}

    pr_info("ILI9225 Driver Ready\n");
    return 0;
}

static void ili_remove(struct spi_device *spi)
{
    device_destroy(ili_class, devno);
    class_destroy(ili_class);
    cdev_del(&ili_cdev);
    unregister_chrdev_region(devno, 1);
pr_info("ILI9225 driver unloaded\n");
}

/* ---------- OF MATCH ---------- */

static const struct of_device_id ili_of_match[] = {
    { .compatible = "display,ili9225" },
    {}
};
MODULE_DEVICE_TABLE(of, ili_of_match);

static struct spi_driver ili_driver = {
    .driver = {
        .name = "ili9225",
        .of_match_table = ili_of_match,
    },
    .probe = ili_probe,
    .remove = ili_remove,
};

module_spi_driver(ili_driver);

MODULE_LICENSE("GPL");

