#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/jiffies.h>

#define I2C_BASE   0xFE205000   // Pi 4 BSC1
#define GPIO_BASE  0xFE200000   // Pi 4 GPIO

#define I2C_SIZE   0x100
#define GPIO_SIZE  0x100

/* BSC Registers */
#define BSC_C      0x00
#define BSC_S      0x04
#define BSC_DLEN   0x08
#define BSC_A      0x0C
#define BSC_FIFO   0x10
#define BSC_DIV    0x14

#define I2CEN  (1 << 15)
#define ST     (1 << 7)
#define CLEAR  (1 << 4)

#define DONE   (1 << 1)
#define TXD    (1 << 4)

#define OLED_ADDR 0x3C

static void __iomem *i2c_base;
static void __iomem *gpio_base;

/* Configure GPIO2 & GPIO3 ALT0 */
static void gpio_i2c_init(void)
{
    u32 val;

    val = readl(gpio_base + 0x00);

    val &= ~((7 << 6) | (7 << 9));  // clear GPIO2,3
    val |=  (4 << 6) | (4 << 9);   // ALT0

    writel(val, gpio_base + 0x00);
}

/* Write two bytes (control + data) */
static int i2c_write(u8 control, u8 data)
{
    unsigned long timeout;

    writel(0xFFFF, i2c_base + BSC_S);
    writel(OLED_ADDR, i2c_base + BSC_A);
    writel(2, i2c_base + BSC_DLEN);

    writel(I2CEN | CLEAR, i2c_base + BSC_C);

    writel(control, i2c_base + BSC_FIFO);
    writel(data, i2c_base + BSC_FIFO);

    writel(I2CEN | ST, i2c_base + BSC_C);

    timeout = jiffies + msecs_to_jiffies(100);
    while (!(readl(i2c_base + BSC_S) & DONE)) {
        if (time_after(jiffies, timeout))
            return -ETIMEDOUT;
        cpu_relax();
    }

    writel(DONE, i2c_base + BSC_S);
    return 0;
}

/* Minimal OLED init */
static void oled_init(void)
{
    i2c_write(0x00, 0xAE);
    i2c_write(0x00, 0x20);
    i2c_write(0x00, 0x00);
    i2c_write(0x00, 0xA8);
    i2c_write(0x00, 0x3F);   // 128x64
    i2c_write(0x00, 0xAF);
}

/* Print A */
static void oled_print_A(void)
{
    u8 font_A[8] = {
        0x18,0x24,0x42,0x42,
        0x7E,0x42,0x42,0x00
    };

    int i;

    i2c_write(0x00, 0xB0);
    i2c_write(0x00, 0x00);
    i2c_write(0x00, 0x10);

    for (i = 0; i < 8; i++)
        i2c_write(0x40, font_A[i]);
}

static int __init my_init(void)
{
    pr_info("Custom I2C Driver Loaded\n");

    i2c_base = ioremap(I2C_BASE, I2C_SIZE);
    gpio_base = ioremap(GPIO_BASE, GPIO_SIZE);

    if (!i2c_base || !gpio_base)
        return -ENOMEM;

    gpio_i2c_init();
    writel(2500, i2c_base + BSC_DIV);

    oled_init();
    msleep(100);
    oled_print_A();

    pr_info("Character A sent\n");
    return 0;
}

static void __exit my_exit(void)
{
    writel(0, i2c_base + BSC_C);
    iounmap(i2c_base);
    iounmap(gpio_base);
    pr_info("Driver Removed\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
