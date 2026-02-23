#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/delay.h>
#include <linux/io.h>

#define DC_GPIO     25
#define RESET_GPIO  24

#define GPIO_BASE_PHYS  0xFE200000
#define GPIO_SIZE       0xB4

#define GPFSEL2 0x08
#define GPSET0  0x1C
#define GPCLR0  0x28

static void __iomem *gpio_base;

/* ---------------- GPIO ---------------- */

static void gpio_set_output(int gpio)
{
    u32 val;
    val = ioread32(gpio_base + GPFSEL2);
    val &= ~(0x7 << ((gpio % 10) * 3));
    val |=  (0x1 << ((gpio % 10) * 3));
    iowrite32(val, gpio_base + GPFSEL2);
}

static void gpio_set(int gpio)
{
    iowrite32(1 << gpio, gpio_base + GPSET0);
}

static void gpio_clear(int gpio)
{
    iowrite32(1 << gpio, gpio_base + GPCLR0);
}

/* ---------------- SPI WRITE (8 BIT CORRECT) ---------------- */

static void ili_write(struct spi_device *spi, u16 data)
{
    u8 tx[2];

    tx[0] = data >> 8;
    tx[1] = data & 0xFF;

    spi_write(spi, tx, 2);
}

static void ili_cmd(struct spi_device *spi, u16 cmd)
{
    gpio_clear(DC_GPIO);
    ili_write(spi, cmd);
}

static void ili_data(struct spi_device *spi, u16 data)
{
    gpio_set(DC_GPIO);
    ili_write(spi, data);
}

/* ---------------- RESET ---------------- */

static void ili_reset(void)
{
    gpio_set(RESET_GPIO);
    mdelay(5);
    gpio_clear(RESET_GPIO);
    mdelay(20);
    gpio_set(RESET_GPIO);
    mdelay(150);
}

/* ---------------- INIT ---------------- */

static void ili_init_display(struct spi_device *spi)
{
    ili_reset();

    ili_cmd(spi, 0x01);
    mdelay(50);

    ili_cmd(spi, 0x11);
    mdelay(120);

    ili_cmd(spi, 0x3A);
    ili_data(spi, 0x55);

    ili_cmd(spi, 0x29);
    mdelay(20);
}

/* ---------------- FILL RED ---------------- */

static void ili_fill_red(struct spi_device *spi)
{
    int i;

    ili_cmd(spi, 0x2C);
    gpio_set(DC_GPIO);

    for (i = 0; i < 176 * 220; i++)
        ili_write(spi, 0xF800);   // RED
}

/* ---------------- PROBE ---------------- */

static int ili_probe(struct spi_device *spi)
{
    printk("ILI9225 PROBE START\n");

    gpio_base = ioremap(GPIO_BASE_PHYS, GPIO_SIZE);
    if (!gpio_base)
        return -ENOMEM;

    gpio_set_output(DC_GPIO);
    gpio_set_output(RESET_GPIO);

    spi->mode = SPI_MODE_0;
    spi->bits_per_word = 8;      // CRITICAL FIX
    spi->max_speed_hz = 8000000;
    spi_setup(spi);

    ili_init_display(spi);
    ili_fill_red(spi);

    printk("ILI9225 SHOULD NOW BE RED\n");

    return 0;
}

static void ili_remove(struct spi_device *spi)
{
    printk("ILI9225 removed\n");
}

/* ---------------- DRIVER ---------------- */

static struct spi_driver ili_driver = {
    .driver = {
        .name = "spidev",   // IMPORTANT: bind to spi0.1
    },
    .probe = ili_probe,
    .remove = ili_remove,
};

module_spi_driver(ili_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("ILI9225 SPI TEST DRIVER");
