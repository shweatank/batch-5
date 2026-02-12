#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/delay.h>

/* ================== CONFIG ================== */

/* Change UART base if needed:
 * Pi 1 / Zero : 0x20201000
 * Pi 2 / 3    : 0x3F201000
 * Pi 4        : 0xFE201000
 */
#define UART_BASE_PHYS   0x3F201000
#define UART_SIZE        0x100

#define DEVICE_NAME "rpi_uart"

/* ================== PL011 REGISTERS ================== */

#define UART_DR     0x00
#define UART_RSR    0x04
#define UART_ECR    0x04
#define UART_FR     0x18
#define UART_IBRD   0x24
#define UART_FBRD   0x28
#define UART_LCRH   0x2C
#define UART_CR     0x30
#define UART_ICR    0x44

/* Flags */
#define FR_TXFF     (1 << 5)
#define FR_RXFE     (1 << 4)

/* Control */
#define CR_UARTEN   (1 << 0)
#define CR_TXE      (1 << 8)
#define CR_RXE      (1 << 9)

/* Line control */
#define LCRH_8BIT   (3 << 5)

/* ================== GLOBALS ================== */

static void __iomem *uart_base;
static dev_t devno;
static struct cdev uart_cdev;
static struct class *uart_class;

/* ================== MMIO HELPERS ================== */

#define uart_read(off)        readl(uart_base + (off))
#define uart_write(val, off)  writel((val), uart_base + (off))

/* ================== UART HW INIT ================== */

static void uart_hw_init(void)
{
    uart_write(0, UART_CR);        /* Disable UART */
    uart_write(0x7FF, UART_ICR);   /* Clear interrupts */

    /* 115200 baud @ 48 MHz UART clock */
    uart_write(26, UART_IBRD);
    uart_write(3,  UART_FBRD);

    /* 8N1, FIFO disabled (important for polling) */
    uart_write(LCRH_8BIT, UART_LCRH);

    uart_write(CR_UARTEN | CR_TXE | CR_RXE, UART_CR);
}

/* ================== LOW-LEVEL TX ================== */

static void uart_putc(char c)
{
    while (uart_read(UART_FR) & FR_TXFF)
        cpu_relax();

    uart_write(c, UART_DR);
}

/* ================== LOW-LEVEL RX ================== */

static int uart_getc(char *c)
{
    u32 dr;

    while (uart_read(UART_FR) & FR_RXFE)
        cpu_relax();

    dr = uart_read(UART_DR);

    /* RX error? clear and retry */
    if (dr & 0xF00) {
        uart_write(0x7FF, UART_ECR);
        return -EIO;
    }

    *c = dr & 0xFF;
    return 0;
}

/* ================== FILE OPS ================== */

static ssize_t uart_write_user(struct file *f,
                               const char __user *buf,
                               size_t len, loff_t *off)
{
    size_t i;
    char c;

    for (i = 0; i < len; i++) {
        if (copy_from_user(&c, buf + i, 1))
            return -EFAULT;

        uart_putc(c);
	printk("data sent : %c\n",c);
	uart_getc(&c);
	printk("data received : %c\n",c);
    }

    return len;
}

static ssize_t uart_read_user(struct file *f,
                              char __user *buf,
                              size_t len, loff_t *off)
{
    size_t i;
    char c;

    for (i = 0; i < len; i++) {
        if (uart_getc(&c))
            break;

        if (copy_to_user(buf + i, &c, 1))
            return -EFAULT;



    }

    return i;
}

static struct file_operations uart_fops = {
    .owner = THIS_MODULE,
    .read  = uart_read_user,
    .write = uart_write_user,
};

/* ================== MODULE INIT ================== */

static int __init uart_init(void)
{
    int ret;

    uart_base = ioremap(UART_BASE_PHYS, UART_SIZE);
    if (!uart_base)
        return -ENOMEM;

    ret = alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME);
    if (ret)
        goto unmap;

    cdev_init(&uart_cdev, &uart_fops);
    ret = cdev_add(&uart_cdev, devno, 1);
    if (ret)
        goto unregister;

    uart_class = class_create(DEVICE_NAME);
    device_create(uart_class, NULL, devno, NULL, DEVICE_NAME);

    uart_hw_init();

    pr_info("rpi_uart: driver loaded\n");
    return 0;

unregister:
    unregister_chrdev_region(devno, 1);
unmap:
    iounmap(uart_base);
    return ret;
}

/* ================== MODULE EXIT ================== */

static void __exit uart_exit(void)
{
    device_destroy(uart_class, devno);
    class_destroy(uart_class);
    cdev_del(&uart_cdev);
    unregister_chrdev_region(devno, 1);
    iounmap(uart_base);
    pr_info("rpi_uart: driver unloaded\n");
}

module_init(uart_init);
module_exit(uart_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("Raspberry Pi PL011 UART driver using ioremap");


