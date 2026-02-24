/*
 * Raspberry Pi 4 SPI TFT Display Driver (ILI9341 Example)
 * No DTS
 * No wiringPi
 * Direct GPIO register access
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/spi/spi.h>
#include <linux/delay.h>

/* ============================= */
/* ===== Raspberry Pi 4 GPIO ==== */
/* ============================= */

#define GPIO_BASE_PHYS   0xFE200000
#define GPIO_SIZE        0xB4

#define GPFSEL0          0x00
#define GPFSEL1          0x04
#define GPSET0           0x1C
#define GPCLR0           0x28

#define DC_PIN           25
#define RST_PIN          24

static void __iomem *gpio_base;
static struct spi_device *display_spi;

/* ============================= */
/* ===== GPIO FUNCTIONS ========= */
/* ============================= */

static void gpio_set_output(int pin)
{
    u32 reg = GPFSEL0 + ((pin / 10) * 4);
    u32 shift = (pin % 10) * 3;
    u32 val;

    val = readl(gpio_base + reg);
    val &= ~(7 << shift);
    val |= (1 << shift);
    writel(val, gpio_base + reg);
}

static void gpio_set(int pin)
{
    writel(1 << pin, gpio_base + GPSET0);
}

static void gpio_clear(int pin)
{
    writel(1 << pin, gpio_base + GPCLR0);
}

/* ============================= */
/* ===== SPI WRITE ============== */
/* ============================= */

static int spi_write_bytes(u8 *buf, int len)
{
    struct spi_transfer t = {
        .tx_buf = buf,
        .len = len,
        .speed_hz = 10000000,
        .bits_per_word = 8,
    };

    struct spi_message m;

    spi_message_init(&m);
    spi_message_add_tail(&t, &m);

    return spi_sync(display_spi, &m);
}

/* ============================= */
/* ===== DISPLAY COMMANDS ====== */
/* ============================= */

static void display_cmd(u8 cmd)
{
    gpio_clear(DC_PIN);
    spi_write_bytes(&cmd, 1);
}

static void display_data(u8 data)
{
    gpio_set(DC_PIN);
    spi_write_bytes(&data, 1);
}

static void display_data_buf(u8 *buf, int len)
{
    gpio_set(DC_PIN);
    spi_write_bytes(buf, len);
}

/* ============================= */
/* ===== DISPLAY RESET ========= */
/* ============================= */

static void display_reset(void)
{
    gpio_clear(RST_PIN);
    msleep(50);
    gpio_set(RST_PIN);
    msleep(50);
}

/* ============================= */
/* ===== ILI9341 INIT ========== */
/* ============================= */

static void display_init(void)
{
    display_reset();

    display_cmd(0x01); // Software Reset
    msleep(150);

    display_cmd(0x28); // Display OFF

    display_cmd(0xCF);
    display_data(0x00);
    display_data(0xC1);
    display_data(0x30);

    display_cmd(0xED);
    display_data(0x64);
    display_data(0x03);
    display_data(0x12);
    display_data(0x81);

    display_cmd(0xE8);
    display_data(0x85);
    display_data(0x00);
    display_data(0x78);

    display_cmd(0xCB);
    display_data(0x39);
    display_data(0x2C);
    display_data(0x00);
    display_data(0x34);
    display_data(0x02);

    display_cmd(0xF7);
    display_data(0x20);

    display_cmd(0xEA);
    display_data(0x00);
    display_data(0x00);

    display_cmd(0xC0);
    display_data(0x23);

    display_cmd(0xC1);
    display_data(0x10);

    display_cmd(0xC5);
    display_data(0x3e);
    display_data(0x28);

    display_cmd(0xC7);
    display_data(0x86);

    display_cmd(0x36);
    display_data(0x48);

    display_cmd(0x3A);
    display_data(0x55);

    display_cmd(0xB1);
    display_data(0x00);
    display_data(0x18);

    display_cmd(0xB6);
    display_data(0x08);
    display_data(0x82);
    display_data(0x27);

    display_cmd(0x11);
    msleep(120);

    display_cmd(0x29); // Display ON
}

/* ============================= */
/* ===== SIMPLE COLOR TEST ===== */
/* ============================= */

static void display_fill_color(u16 color)
{
    int i;
    u8 data[2];

    display_cmd(0x2A);
    display_data(0x00); display_data(0x00);
    display_data(0x00); display_data(0xEF);

    display_cmd(0x2B);
    display_data(0x00); display_data(0x00);
    display_data(0x01); display_data(0x3F);

    display_cmd(0x2C);

    data[0] = color >> 8;
    data[1] = color & 0xFF;

    for (i = 0; i < 320 * 240; i++)
        display_data_buf(data, 2);
}

/* ============================= */
/* ===== SPI PROBE ============= */
/* ============================= */

static int display_probe(struct spi_device *spi)
{
    display_spi = spi;

    gpio_base = ioremap(GPIO_BASE_PHYS, GPIO_SIZE);
    if (!gpio_base)
        return -ENOMEM;

    gpio_set_output(DC_PIN);
    gpio_set_output(RST_PIN);

    display_init();

    display_fill_color(0xF800); // RED TEST

    pr_info("SPI TFT Display Driver Loaded\n");
    return 0;
}

static void display_remove(struct spi_device *spi)
{
    if (gpio_base)
    	iounmap(gpio_base);

    pr_info("SPI TFT Display Driver Removed\n");
}

/* ============================= */
/* ===== SPI DRIVER STRUCT ===== */
/* ============================= */

static struct spi_driver display_driver = {
    .driver = {
        .name = "rpi_spi_display",
        .owner = THIS_MODULE,
    },
    .probe = display_probe,
    .remove = display_remove,
};

/* ============================= */
/* ===== MODULE INIT/EXIT ====== */
/* ============================= */

static int __init display_init_module(void)
{
    return spi_register_driver(&display_driver);
}

static void __exit display_exit_module(void)
{
    spi_unregister_driver(&display_driver);
}

module_init(display_init_module);
module_exit(display_exit_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ajay Kumar");
MODULE_DESCRIPTION("Raspberry Pi 4 SPI TFT Driver without DTS");

