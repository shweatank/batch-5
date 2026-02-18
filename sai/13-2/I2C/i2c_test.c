#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/jiffies.h>

#define GPIO_BASE   0xFE200000
#define BSC1_BASE   0xFE205000

#define GPIO_SIZE   0x100
#define BSC_SIZE    0x100

/* BSC Registers */
#define BSC_C       0x00
#define BSC_S       0x04
#define BSC_DLEN    0x08
#define BSC_A       0x0C
#define BSC_FIFO    0x10
#define BSC_DIV     0x14

#define I2CEN   (1 << 15)
#define ST      (1 << 7)
#define CLEAR   (1 << 4)

#define DONE    (1 << 1)
#define TXD     (1 << 4)

#define OLED_ADDR 0x3C

static void __iomem *gpio_base;
static void __iomem *bsc_base;

/* Configure GPIO2 & 3 to ALT0 (I2C1) */
static void gpio_i2c_init(void)
{
    u32 val;

    val = readl(gpio_base + 0x00);
    val &= ~((7 << 6) | (7 << 9));
    val |=  (4 << 6) | (4 << 9);  // ALT0
    writel(val, gpio_base + 0x00);
}

/* Low level I2C write */
static int bsc_write(u8 *data, int len)
{
    unsigned long timeout;
    int i;

    writel(0xFFFF, bsc_base + BSC_S);
    writel(OLED_ADDR, bsc_base + BSC_A);
    writel(len, bsc_base + BSC_DLEN);

    writel(I2CEN | CLEAR, bsc_base + BSC_C);

    for (i = 0; i < len; i++)
        writel(data[i], bsc_base + BSC_FIFO);

    writel(I2CEN | ST, bsc_base + BSC_C);

    timeout = jiffies + msecs_to_jiffies(100);

    while (!(readl(bsc_base + BSC_S) & DONE)) {
        if (time_after(jiffies, timeout))
            return -ETIMEDOUT;
        cpu_relax();
    }

    writel(DONE, bsc_base + BSC_S);
    return 0;
}

static void oled_cmd(u8 cmd)
{
    u8 buf[2] = {0x00, cmd};
    bsc_write(buf, 2);
}

static void oled_data(u8 data)
{
    u8 buf[2] = {0x40, data};
    bsc_write(buf, 2);
}

static void oled_init_display(void)
{
    oled_cmd(0xAE);
    oled_cmd(0xA8); oled_cmd(0x1F);
    oled_cmd(0xD3); oled_cmd(0x00);
    oled_cmd(0x40);
    oled_cmd(0x8D); oled_cmd(0x14);
    oled_cmd(0x20); oled_cmd(0x00);
    oled_cmd(0xA1);
    oled_cmd(0xC8);
    oled_cmd(0xDA); oled_cmd(0x02);
    oled_cmd(0x81); oled_cmd(0xCF);
    oled_cmd(0xD9); oled_cmd(0xF1);
    oled_cmd(0xDB); oled_cmd(0x40);
    oled_cmd(0xA4);
    oled_cmd(0xA6);
    oled_cmd(0xAF);
}

static void oled_print_A(void)
{
    u8 font_A[8] = {
        0x18,0x24,0x42,0x42,
        0x7E,0x42,0x42,0x00
    };
    int i;

    oled_cmd(0x21);
    oled_cmd(0x00);
    oled_cmd(0x07);

    oled_cmd(0x22);
    oled_cmd(0x00);
    oled_cmd(0x00);

    for (i = 0; i < 8; i++)
        oled_data(font_A[i]);
}

static int __init my_init(void)
{
    pr_info("MMIO I2C OLED driver loading\n");

    gpio_base = ioremap(GPIO_BASE, GPIO_SIZE);
    bsc_base  = ioremap(BSC1_BASE, BSC_SIZE);

    if (!gpio_base || !bsc_base)
        return -ENOMEM;

    gpio_i2c_init();

    writel(2500, bsc_base + BSC_DIV);  // ~100kHz

    oled_init_display();
    msleep(100);
    oled_print_A();

    pr_info("A printed using MMIO\n");
    return 0;
}

static void __exit my_exit(void)
{
    writel(0, bsc_base + BSC_C);
    iounmap(gpio_base);
    iounmap(bsc_base);
    pr_info("Driver removed\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("Bare-metal style I2C MMIO driver for SSD1306");
