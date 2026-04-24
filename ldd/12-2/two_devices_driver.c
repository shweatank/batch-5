#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/device.h>

#define UART_BASE  0xFE201000   // Pi4
#define UART_SIZE  0x100

/* Registers */
#define UART_DR    0x00
#define UART_FR    0x18
#define UART_IBRD  0x24
#define UART_FBRD  0x28
#define UART_LCRH  0x2C
#define UART_CR    0x30
#define UART_ICR   0x44

/* Flags */
#define FR_TXFF (1<<5)
#define FR_RXFE (1<<4)

/* Control */
#define CR_UARTEN (1<<0)
#define CR_TXE    (1<<8)
#define CR_RXE    (1<<9)

/* Line */
#define LCRH_8BIT (3<<5)
#define LCRH_FEN  (1<<4)

#define DEVICE_NAME "rpi_uart"

static dev_t devno;
static struct cdev uart_cdev;
static struct class *uart_class;

static void __iomem *uart_base;

#define uart_read(o)  readl(uart_base + o)
#define uart_write(v,o) writel(v, uart_base + o)

/* UART Init */
static void uart_hw_init(void)
{
    uart_write(0, UART_CR);
    uart_write(0x7FF, UART_ICR);

    /* 115200 baud */
    uart_write(26, UART_IBRD);
    uart_write(3, UART_FBRD);

    uart_write(LCRH_8BIT | LCRH_FEN, UART_LCRH);

    uart_write(CR_UARTEN | CR_TXE | CR_RXE, UART_CR);

    pr_info("UART Initialized\n");
}


/* WRITE (TX) */
static ssize_t uart_write_user(struct file *f,
        const char __user *buf,
        size_t len, loff_t *off)
{
    char ch;
    int i;

    for(i=0;i<len;i++)
    {
        if(copy_from_user(&ch, buf+i, 1))
            return -EFAULT;

        while(uart_read(UART_FR) & FR_TXFF);

        uart_write(ch, UART_DR);
pr_info("data sent=%c\n",ch);
    }

    return len;
}


/* READ (RX) - BLOCKING */
static ssize_t uart_read_user(struct file *f,
        char __user *buf,
        size_t len, loff_t *off)
{
    char ch;
    int i;

    for(i=0;i<len;i++)
    {
        if(uart_read(UART_FR) & FR_RXFE)
          break;

        ch = uart_read(UART_DR) & 0xFF;

        if(copy_to_user(buf+i, &ch, 1))
            return -EFAULT;

        /* Stop at newline */
        if(ch == '\0')
            break;

pr_info("data recieved=%c\n",ch);
    }

    return i+1;
}


/* File Ops */
static struct file_operations uart_fops =
{
    .owner = THIS_MODULE,
    .read  = uart_read_user,
    .write = uart_write_user,
};


/* INIT */
static int __init uart_init(void)
{
    int ret;

    uart_base = ioremap(UART_BASE, UART_SIZE);
    if(!uart_base)
        return -ENOMEM;

    ret = alloc_chrdev_region(&devno,0,1,DEVICE_NAME);
    if(ret)
        return ret;

    cdev_init(&uart_cdev,&uart_fops);
    cdev_add(&uart_cdev,devno,1);

    uart_class = class_create(DEVICE_NAME);
    device_create(uart_class,NULL,devno,NULL,DEVICE_NAME);

    uart_hw_init();

    pr_info("rpi_uart loaded\n");
    return 0;
}


/* EXIT */
static void __exit uart_exit(void)
{
    device_destroy(uart_class,devno);
    class_destroy(uart_class);

    cdev_del(&uart_cdev);
    unregister_chrdev_region(devno,1);

    iounmap(uart_base);

    pr_info("rpi_uart removed\n");
}

module_init(uart_init);
module_exit(uart_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Saikumar");
MODULE_DESCRIPTION("UART Driver");
