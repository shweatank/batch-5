#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/delay.h>

/* SPI0 MMIO base for Raspberry Pi 4 (BCM2711) */
#define SPI_BASE   0xFE204000
#define SPI_SIZE   0x100

/* SPI registers */
#define CS     0x00
#define FIFO   0x04
#define CLK    0x08
#define DLEN   0x0C

/* CS register bits */
#define CS_TA      (1 << 7)
#define CS_CLEAR   (3 << 4)
#define CS_RXD     (1 << 17)
#define CS_TXD     (1 << 18)

/* EEPROM commands (25xx series) */
#define EEPROM_WREN   0x06
#define EEPROM_WRITE  0x02
#define EEPROM_READ   0x03

#define DEVICE_NAME "spi_data"

static int major;
static void __iomem *spi_base;

/* ---------- SPI TRANSFER HELPER ---------- */
static u8 spi_transfer(u8 tx)
{
    /* Wait until TX FIFO can accept data */
    while (!(readl(spi_base + CS) & CS_TXD))
        cpu_relax();

    writel(tx, spi_base + FIFO);

    /* Wait until RX FIFO has data */
    while (!(readl(spi_base + CS) & CS_RXD))
        cpu_relax();

    return readl(spi_base + FIFO) & 0xFF;
}

/* ---------- EEPROM HELPERS ---------- */

/* Enable EEPROM write */
static void spi_write_enable(void)
{
    writel(CS_CLEAR, spi_base + CS);  // Clear FIFOs
    writel(readl(spi_base + CS) | CS_TA, spi_base + CS); // Assert CS

    spi_transfer(EEPROM_WREN);

    writel(readl(spi_base + CS) & ~CS_TA, spi_base + CS); // Deassert CS
}

/* Write 1 byte to EEPROM at address */
static void eeprom_write_byte(u16 addr, u8 data)
{
    spi_write_enable();

    writel(CS_CLEAR, spi_base + CS);  // Clear FIFOs
    writel(readl(spi_base + CS) | CS_TA, spi_base + CS); // Assert CS

    spi_transfer(EEPROM_WRITE);
    spi_transfer(addr >> 8);       // MSB
    spi_transfer(addr & 0xFF);     // LSB
    spi_transfer(data);

    writel(readl(spi_base + CS) & ~CS_TA, spi_base + CS); // Deassert CS

    mdelay(5); // EEPROM write cycle
}

/* Read 1 byte from EEPROM at address */
static u8 eeprom_read_byte(u16 addr)
{
    u8 data;

    writel(CS_CLEAR, spi_base + CS); // Clear FIFOs
    writel(readl(spi_base + CS) | CS_TA, spi_base + CS); // Assert CS

    spi_transfer(EEPROM_READ);
    spi_transfer(addr >> 8);         // MSB
    spi_transfer(addr & 0xFF);       // LSB
    data = spi_transfer(0xFF);       // Dummy write to read

    writel(readl(spi_base + CS) & ~CS_TA, spi_base + CS); // Deassert CS

    return data;
}

/* ---------- FILE OPERATIONS ---------- */

/* Read from EEPROM */
static ssize_t spi_read(struct file *file,
                        char __user *buf,
                        size_t len,
                        loff_t *off)
{
    size_t i;
    u8 data;

    for (i = 0; i < len; i++) {
        data = eeprom_read_byte(*off + i);

        if (copy_to_user(&buf[i], &data, 1))
            return -EFAULT;
    }

    *off += len;
    return len;
}

/* Write to EEPROM */
static ssize_t spi_write(struct file *file,
                         const char __user *buf,
                         size_t len,
                         loff_t *off)
{
    size_t i;
    u8 data;

    for (i = 0; i < len; i++) {
        if (copy_from_user(&data, &buf[i], 1))
            return -EFAULT;

        eeprom_write_byte(*off + i, data);
    }

    *off += len;
    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read  = spi_read,
    .write = spi_write,
};

/* ---------- SPI HARDWARE INIT ---------- */
static void spi_hw_init(void)
{
    writel(0, spi_base + CS);       // Disable SPI
    writel(CS_CLEAR, spi_base + CS); // Clear FIFOs
    writel(100, spi_base + CLK);    // Clock divider (~2.5 MHz)
    pr_info("SPI hardware initialized\n");
}

/* ---------- MODULE INIT ---------- */
static int __init spi_init_module(void)
{
    spi_base = ioremap(SPI_BASE, SPI_SIZE);
    if (!spi_base)
        return -ENOMEM;

    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        iounmap(spi_base);
        return major;
    }

    spi_hw_init();

    pr_info("SPI EEPROM driver loaded (major=%d)\n", major);
    pr_info("Device: /dev/%s\n", DEVICE_NAME);

    return 0;
}

/* ---------- MODULE EXIT ---------- */
static void __exit spi_exit_module(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    iounmap(spi_base);
    pr_info("SPI EEPROM driver unloaded\n");
}

module_init(spi_init_module);
module_exit(spi_exit_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KISHORE");
MODULE_DESCRIPTION("Full-featured SPI EEPROM driver for Raspberry Pi 4");

