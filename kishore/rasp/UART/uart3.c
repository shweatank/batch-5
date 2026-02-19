#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/device.h>
#include <linux/delay.h>

/* BCM2711 PL011 UART0 base */
#define UART_BASE  0xFE201000
#define UART_SIZE  0x100

/* Registers */
#define UART_DR    0x00
#define UART_FR    0x18
#define UART_IBRD  0x24
#define UART_FBRD  0x28
#define UART_LCRH  0x2C
#define UART_CR    0x30
#define UART_IMSC  0x38
#define UART_ICR   0x44

/* Flags */
#define FR_TXFF (1 << 5)
#define FR_RXFE (1 << 4)
#define FR_BUSY (1 << 3)

/* Control bits */
#define CR_UARTEN (1 << 0)
#define CR_TXE    (1 << 8)
#define CR_RXE    (1 << 9)

/* Line control */
#define LCRH_8BIT (3 << 5)
#define LCRH_FEN  (1 << 4)

#define DEVICE_NAME "rpi_uart"

static dev_t devno;
static struct cdev uart_cdev;
static struct class *uart_class;

static void __iomem *uart_base;

/* MMIO helpers */
#define uart_read(off)        readl(uart_base + (off))
#define uart_write(val, off)  writel((val), uart_base + (off))

/* ---------------- UART INIT ---------------- */
static void uart_hw_init(void)
{
    uart_write(0, UART_CR);        /* Disable UART */
    uart_write(0x7FF, UART_ICR);   /* Clear IRQs */

    /* 115200 baud @ 48MHz */
    uart_write(26, UART_IBRD);
    uart_write(3,  UART_FBRD);

    uart_write(LCRH_8BIT | LCRH_FEN, UART_LCRH);

    uart_write(CR_UARTEN | CR_TXE | CR_RXE, UART_CR);
    uart_write(0, UART_IMSC);      /* No interrupts */

    pr_info("rpi_uart: UART initialized\n");
}

/* ---------------- FILE OPS ---------------- */
static ssize_t uart_write_user(struct file *f,
                               const char __user *buf,
                               size_t len, loff_t *off)
{
    char ch;
    size_t i;
pr_info("bye.............\n");
    for (i = 0; i < len; i++) {
        if (copy_from_user(&ch, buf + i, 1))
            return -EFAULT;

        while (uart_read(UART_FR) & FR_TXFF)
            cpu_relax();

        uart_write(ch, UART_DR);
    }
    pr_info("write sent........\n");
    return len;
}

static ssize_t uart_read_user(struct file *f,
                              char __user *buf,
                              size_t len, loff_t *off)
{
size_t i;
for(i=0;i<len;i++){

    

/*	char ch,ch1[len];
    size_t i;
pr_info("hai..........\n");
    for (i = 0; i < len; i++) {
        if (uart_read(UART_FR) & FR_RXFE)
            break;

        ch = uart_read(UART_DR) & 0xFF;
	uart_write(ch,UART_DR);

        if (copy_to_user(buf + i, &ch, 1))
            return -EFAULT;
    }
    */
    return i;
}

static struct file_operations uart_fops = {
    .owner = THIS_MODULE,
    .read  = uart_read_user,
    .write = uart_write_user,
};

/* ---------------- MODULE INIT ---------------- */
static int __init uart_init(void)
{
    int ret;

    uart_base = ioremap(UART_BASE, UART_SIZE);
    if (!uart_base) {
        pr_err("rpi_uart: ioremap failed\n");
        return -ENOMEM;
    }

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

    pr_info("rpi_uart: loaded (ioremap only)\n");
    return 0;

unregister:
    unregister_chrdev_region(devno, 1);
unmap:
    iounmap(uart_base);
    return ret;
}

/* ---------------- MODULE EXIT ---------------- */
static void __exit uart_exit(void)
{
    device_destroy(uart_class, devno);
    class_destroy(uart_class);
    cdev_del(&uart_cdev);
    unregister_chrdev_region(devno, 1);
    iounmap(uart_base);

    pr_info("rpi_uart: unloaded\n");
}

module_init(uart_init);
module_exit(uart_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("techdhaba");
MODULE_DESCRIPTION("Raspberry Pi UART driver using ioremap only");
