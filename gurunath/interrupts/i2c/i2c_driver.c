#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/delay.h>

#define DRIVER_NAME "bcm2711_i2c_raw"

/* Raspberry Pi 4 peripheral base */
#define PERIPH_BASE  0xFE000000

/* BSC1 (I2C1) base */
#define BSC1_BASE    (PERIPH_BASE + 0x804000)
#define BSC1_SIZE    0x20

/* Register offsets */
#define BSC_C        0x00
#define BSC_S        0x04
#define BSC_DLEN     0x08
#define BSC_A        0x0C
#define BSC_FIFO     0x10
#define BSC_DIV      0x14

/* Control register bits */
#define BSC_C_I2CEN  (1 << 15)
#define BSC_C_ST     (1 << 7)
#define BSC_C_CLEAR  (1 << 4)

/* Status register bits */
#define BSC_S_DONE   (1 << 1)
#define BSC_S_TXD    (1 << 4)
#define BSC_S_ERR    (1 << 8)
#define BSC_S_CLKT   (1 << 9)

static void __iomem *bsc1_base;

/* Write helper */
static inline void bsc_writel(u32 val, u32 reg)
{
    writel(val, bsc1_base + reg);
}

/* Read helper */
static inline u32 bsc_readl(u32 reg)
{
    return readl(bsc1_base + reg);
}

/* Simple I2C write: send one byte */
static int i2c_write_byte(u8 slave, u8 data)
{
    /* Clear status */
    bsc_writel(BSC_S_DONE | BSC_S_ERR | BSC_S_CLKT, BSC_S);

    /* Set slave address */
    bsc_writel(slave, BSC_A);

    /* Data length */
    bsc_writel(1, BSC_DLEN);

    /* Write data */
    bsc_writel(data, BSC_FIFO);

    /* Start transfer */
    bsc_writel(BSC_C_I2CEN | BSC_C_ST | BSC_C_CLEAR, BSC_C);

    /* Wait for completion */
    while (!(bsc_readl(BSC_S) & BSC_S_DONE))
        cpu_relax();

    /* Check errors */
    if (bsc_readl(BSC_S) & (BSC_S_ERR | BSC_S_CLKT))
        return -EIO;

    return 0;
}

static int __init bcm2711_i2c_init(void)
{
    int ret;

    pr_info("%s: init\n", DRIVER_NAME);

    /* Map I2C registers */
    bsc1_base = ioremap(BSC1_BASE, BSC1_SIZE);
    if (!bsc1_base)
        return -ENOMEM;

    /* Set clock divider (100kHz approx) */
    bsc_writel(2500, BSC_DIV);

    /* Example: write 0x55 to slave 0x50 */
    ret = i2c_write_byte(0x50, 0x55);
    if (ret)
        pr_err("I2C write failed\n");
    else
        pr_info("I2C write success\n");

    return 0;
}

static void __exit bcm2711_i2c_exit(void)
{
    pr_info("%s: exit\n", DRIVER_NAME);

    if (bsc1_base)
        iounmap(bsc1_base);
}

module_init(bcm2711_i2c_init);
module_exit(bcm2711_i2c_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("Raw I2C driver for Raspberry Pi 4 using ioremap");

