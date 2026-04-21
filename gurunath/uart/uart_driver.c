#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/clk.h>
#include<linux/delay.h>

#define UART_DR     0x00
#define UART_FR     0x18
#define UART_IBRD   0x24
#define UART_FBRD   0x28
#define UART_LCRH   0x2C
#define UART_CR     0x30

#define FR_TXFF     (1 << 5)
#define FR_RXFE     (1 << 4)

struct my_uart {
    void __iomem *base;
    struct cdev cdev;
    dev_t devt;
    struct class *class;
    struct device *device;
    struct clk *clk;
};

/* Initialize UART2 at 115200 baud, 8N1 */
#include <linux/delay.h>

static void uart_init(struct my_uart *dev)
{
    unsigned long clk_rate;
    u32 ibrd, fbrd;
    u32 baud = 115200;
    u32 reg;

    /* 1️⃣ Disable UART before configuration */
    writel(0x0, dev->base + UART_CR);

    /* 2️⃣ Wait until UART not busy */
    while (readl(dev->base + UART_FR) & (1 << 3))
        cpu_relax();   // Wait for BUSY bit to clear

    /* 3️⃣ Clear pending errors (optional but recommended) */
    readl(dev->base + UART_DR);

    /* 4️⃣ Get actual UART clock */
    clk_rate = clk_get_rate(dev->clk);
    pr_info("UART2 clock rate = %lu\n", clk_rate);

    /*
     * Baud divisor calculation:
     * BaudDivisor = UARTCLK / (16 × BaudRate)
     * IBRD = integer part
     * FBRD = round(fractional × 64)
     */
    ibrd = clk_rate / (16 * baud);

    fbrd = ((clk_rate % (16 * baud)) * 64 + (baud / 2)) / baud;

    writel(ibrd, dev->base + UART_IBRD);
    writel(fbrd, dev->base + UART_FBRD);

    pr_info("IBRD=%u FBRD=%u\n", ibrd, fbrd);

    /* 5️⃣ Configure Line Control:
     * - 8 bits
     * - No parity
     * - 1 stop bit
     * - FIFO enabled
     */
    reg = (3 << 5) | (1 << 4);   // WLEN=8bit, FEN=1
    writel(reg, dev->base + UART_LCRH);

    /* 6️⃣ Enable UART, TX, RX */
    reg = (1 << 9) | (1 << 8) | (1 << 0);  // RXE | TXE | UARTEN
    writel(reg, dev->base + UART_CR);

    /* Small delay to stabilize */
    udelay(10);

    pr_info("UART2 initialized at %u baud\n", baud);
}


/* Send a single character */
static void uart_send(struct my_uart *dev, char c)
{
    while (readl(dev->base + UART_FR) & FR_TXFF)
        cpu_relax();
    writel(c, dev->base + UART_DR);
}

/* Receive multiple bytes from FIFO */
static int uart_recv_bytes(struct my_uart *dev, char *buf, size_t max_len)
{
    size_t i = 0;

    while (i < max_len) {
        u32 fr = readl(dev->base + UART_FR);
        if (fr & FR_RXFE) // FIFO empty
            break;
        buf[i++] = readl(dev->base + UART_DR) & 0xFF;
    }

    return i; // number of bytes read
}

/* File operations: read */
static ssize_t my_read(struct file *file, char __user *buf,
                       size_t len, loff_t *off)
{
    struct my_uart *dev = file->private_data;
    char kbuf[128];
    int i = 0;

    if (len > sizeof(kbuf))
        len = sizeof(kbuf);

    /* Wait for first byte */
    while (readl(dev->base + UART_FR) & FR_RXFE)
        cpu_relax();

    /* Small delay to allow FIFO to fill */
    udelay(200);   // 200 microseconds

    /* Now read everything available */
    while (i < len) {
        if (readl(dev->base + UART_FR) & FR_RXFE)
            break;

        kbuf[i++] = readl(dev->base + UART_DR) & 0xFF;
    }

    if (copy_to_user(buf, kbuf, i))
        return -EFAULT;

    return i;
}



/* File operations: write */
static ssize_t my_write(struct file *file,
                        const char __user *buf,
                        size_t len, loff_t *off)
{
    struct my_uart *dev = file->private_data;
    char kbuf[128];
    int i;

    if (len > sizeof(kbuf))
        len = sizeof(kbuf);

    if (copy_from_user(kbuf, buf, len))
        return -EFAULT;

    for (i = 0; i < len; i++)
        uart_send(dev, kbuf[i]);

    return len;
}

/* File operations: open */
static int my_open(struct inode *inode, struct file *file)
{
    struct my_uart *dev = container_of(inode->i_cdev, struct my_uart, cdev);
    file->private_data = dev;
    return 0;
}

/* File operations structure */
static const struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .open  = my_open,
    .read  = my_read,
    .write = my_write,
};

/* Platform driver probe */
static int my_uart_probe(struct platform_device *pdev)
{
    struct my_uart *dev;
    struct resource *res;
    int ret;

    dev = devm_kzalloc(&pdev->dev, sizeof(*dev), GFP_KERNEL);
    if (!dev)
        return -ENOMEM;

    /* Map UART2 registers */
    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    dev->base = devm_ioremap_resource(&pdev->dev, res);
    if (IS_ERR(dev->base))
        return PTR_ERR(dev->base);

    /* Optional: enable clock */
    dev->clk = devm_clk_get(&pdev->dev, NULL);
    if (!IS_ERR(dev->clk))
        clk_prepare_enable(dev->clk);

    uart_init(dev);

    /* Allocate char device */
    ret = alloc_chrdev_region(&dev->devt, 0, 1, "my_uart");
    if (ret)
        return ret;

    cdev_init(&dev->cdev, &my_fops);
    ret = cdev_add(&dev->cdev, dev->devt, 1);
    if (ret) {
        unregister_chrdev_region(dev->devt, 1);
        return ret;
    }

    /* Create device class and device node */
    dev->class = class_create("my_uart_class");
    if (IS_ERR(dev->class)) {
        cdev_del(&dev->cdev);
        unregister_chrdev_region(dev->devt, 1);
        return PTR_ERR(dev->class);
    }

    dev->device = device_create(dev->class, NULL, dev->devt, NULL, "my_uart");
    if (IS_ERR(dev->device)) {
        class_destroy(dev->class);
        cdev_del(&dev->cdev);
        unregister_chrdev_region(dev->devt, 1);
        return PTR_ERR(dev->device);
    }

    platform_set_drvdata(pdev, dev);
    pr_info("UART2 driver loaded, major=%d, minor=%d\n",
            MAJOR(dev->devt), MINOR(dev->devt));

    return 0;
}

/* Platform driver remove */
static void my_uart_remove(struct platform_device *pdev)
{
    struct my_uart *dev = platform_get_drvdata(pdev);

    device_destroy(dev->class, dev->devt);
    class_destroy(dev->class);
    cdev_del(&dev->cdev);
    unregister_chrdev_region(dev->devt, 1);

    pr_info("UART2 driver removed\n");
//    return 0;
}

/* Device tree match table */
static const struct of_device_id my_uart_of_match[] = {
    { .compatible = "my,uart" },
    {}
};
MODULE_DEVICE_TABLE(of, my_uart_of_match);

/* Platform driver structure */
static struct platform_driver my_uart_driver = {
    .probe = my_uart_probe,
    .remove = my_uart_remove,
    .driver = {
        .name = "my_uart",
        .of_match_table = my_uart_of_match,
    },
};

module_platform_driver(my_uart_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KISHORE");
MODULE_DESCRIPTION("UART2 driver for Raspberry Pi 4 with multi-byte FIFO");
