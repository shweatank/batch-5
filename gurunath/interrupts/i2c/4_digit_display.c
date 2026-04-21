#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/delay.h>

#define GPIO_BASE   0xFE200000
#define GPIO_SIZE   0x100

#define GPFSEL0     0x00
#define GPFSEL1     0x04
#define GPSET0      0x1C
#define GPCLR0      0x28
#define GPLEV0      0x34

#define CLK 17
#define DIO 27

static void __iomem *gpio;

/* ---------- GPIO Helpers ---------- */

static void gpio_output(int pin)
{
    u32 reg = (pin / 10) * 4;
    u32 shift = (pin % 10) * 3;
    u32 val = readl(gpio + GPFSEL0 + reg);

    val &= ~(7 << shift);
    val |=  (1 << shift);   // set as output

    writel(val, gpio + GPFSEL0 + reg);
}

static void gpio_input(int pin)
{
    u32 reg = (pin / 10) * 4;
    u32 shift = (pin % 10) * 3;
    u32 val = readl(gpio + GPFSEL0 + reg);

    val &= ~(7 << shift);   // set as input

    writel(val, gpio + GPFSEL0 + reg);
}

static void clk_high(void) { writel(1 << CLK, gpio + GPSET0); }
static void clk_low(void)  { writel(1 << CLK, gpio + GPCLR0); }
static void dio_high(void) { writel(1 << DIO, gpio + GPSET0); }
static void dio_low(void)  { writel(1 << DIO, gpio + GPCLR0); }

/* ---------- TM1637 Protocol ---------- */

static void tm_start(void)
{
    dio_high();
    clk_high();
    udelay(5);
    dio_low();
}

static void tm_stop(void)
{
    clk_low();
    udelay(5);
    dio_low();
    clk_high();
    udelay(2);
    dio_high();
}

static void tm_write_byte(u8 data)
{
    int i;

    for (i = 0; i < 8; i++) {
        clk_low();

        if (data & 0x01)
            dio_high();
        else
            dio_low();

        udelay(3);
        clk_high();
        udelay(3);

        data >>= 1;  // LSB first
    }

    /* ACK */
    clk_low();
    gpio_input(DIO);
    udelay(5);
    clk_high();
    udelay(5);
    gpio_output(DIO);
}

/* 7-segment codes */
static const u8 seg[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

static void tm_display_1234(void)
{
    tm_start();
    tm_write_byte(0x40);  // auto increment
    tm_stop();

    tm_start();
    tm_write_byte(0xC0);  // address 0
    tm_write_byte(seg[1]);
    tm_write_byte(seg[2]);
    tm_write_byte(seg[3]);
    tm_write_byte(seg[4]);
    tm_stop();

    tm_start();
    tm_write_byte(0x8F);  // display ON, max brightness
    tm_stop();
}

/* ---------- Module ---------- */

static int __init tm_init(void)
{
    gpio = ioremap(GPIO_BASE, GPIO_SIZE);
    if (!gpio)
        return -ENOMEM;

    gpio_output(CLK);
    gpio_output(DIO);

    clk_high();
    dio_high();

    tm_display_1234();

    pr_info("TM1637 HW-069 initialized\n");
    return 0;
}

static void __exit tm_exit(void)
{
    iounmap(gpio);
    pr_info("TM1637 module removed\n");
}

module_init(tm_init);
module_exit(tm_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kavya");
MODULE_DESCRIPTION("HW-069 TM1637 using GPIO + ioremap");

