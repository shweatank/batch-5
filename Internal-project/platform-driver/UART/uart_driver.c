#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/clk.h>

#define UART_DR     0x00
#define UART_FR     0x18
#define UART_IBRD   0x24
#define UART_FBRD   0x28
#define UART_LCRH   0x2C
#define UART_CR     0x30

#define FR_TXFF     (1 << 5)
#define FR_RXFE     (1 << 4)

#define UART_CLOCK_DEFAULT 48000000
#define BAUDRATE 115200

struct my_uart {
    void __iomem *base;
    struct cdev cdev;
    dev_t devt;
    struct class *class;
    struct device *device;
    struct clk *clk;
};

static void uart_init(struct my_uart *dev)
{
    unsigned long uart_clk = UART_CLOCK_DEFAULT;
    unsigned int baud_div;
    unsigned int ibrd, fbrd;

    /* Disable UART */
    writel(0x0, dev->base + UART_CR);

    if (!IS_ERR(dev->clk))
        uart_clk = clk_get_rate(dev->clk);

    /* Baud rate calculation */
    baud_div = uart_clk / (16 * BAUDRATE);
    ibrd = baud_div;
    fbrd = ((uart_clk % (16 * BAUDRATE)) * 64 + (BAUDRATE / 2)) / BAUDRATE;

    writel(ibrd, dev->base + UART_IBRD);
    writel(fbrd, dev->base + UART_FBRD);

    /* 8-bit, FIFO enabled */
    writel((3 << 5) | (1 << 4), dev->base + UART_LCRH);

    /* Enable UART, TX, RX */
    writel((1 << 9) | (1 << 8) | 1, dev->base + UART_CR);

    pr_info("UART initialized. clk=%lu baud=%d\n", uart_clk, BAUDRATE);
}

static void uart_send(struct my_uart *dev, char c)
{
    while (readl(dev->base + UART_FR) & FR_TXFF)
        cpu_relax();

    writel(c, dev->base + UART_DR);
}

static int uart_recv(struct my_uart *dev, char *c)
{
    if (readl(dev->base + UART_FR) & FR_RXFE)
        return -EAGAIN;

    *c = readl(dev->base + UART_DR);
    return 0;
}

/* ---------- File Operations ---------- */

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
    pr_info("Data write is %s\n",kbuf);

    return len;
}

static ssize_t my_read(struct file *file,
                       char __user *buf,
                       size_t len, loff_t *off)
{
    struct my_uart *dev = file->private_data;
    char kbuf[128];
    int count = 0;

    if (len > sizeof(kbuf))
        len = sizeof(kbuf);

    while (count < len) {
        if (uart_recv(dev, &kbuf[count]) == 0)
            count++;
        else
            break;  /* No more data */
    }

    if (count == 0)
        return 0;

    if (copy_to_user(buf, kbuf, count))
        return -EFAULT;
    pr_info("Data read is %s\n",buf);

    return count;
}

static int my_open(struct inode *inode, struct file *file)
{
    struct my_uart *dev =
        container_of(inode->i_cdev, struct my_uart, cdev);

    file->private_data = dev;
    return 0;
}

static const struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .open  = my_open,
    .read  = my_read,
    .write = my_write,
};

/* ---------- Platform Driver ---------- */

static int my_uart_probe(struct platform_device *pdev)
{
    struct my_uart *dev;
    struct resource *res;
    int ret;

    dev = devm_kzalloc(&pdev->dev, sizeof(*dev), GFP_KERNEL);
    if (!dev)
        return -ENOMEM;

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    dev->base = devm_ioremap_resource(&pdev->dev, res);
    if (IS_ERR(dev->base))
        return PTR_ERR(dev->base);

    dev->clk = devm_clk_get(&pdev->dev, NULL);
    if (!IS_ERR(dev->clk))
        clk_prepare_enable(dev->clk);

    uart_init(dev);

    ret = alloc_chrdev_region(&dev->devt, 0, 1, "my_uart");
    if (ret)
        return ret;

    cdev_init(&dev->cdev, &my_fops);
    ret = cdev_add(&dev->cdev, dev->devt, 1);
    if (ret)
        goto unregister_region;

    dev->class = class_create("my_uart_class");
    if (IS_ERR(dev->class)) {
        ret = PTR_ERR(dev->class);
        goto del_cdev;
    }

    dev->device =
        device_create(dev->class, NULL, dev->devt, NULL, "my_uart");
    if (IS_ERR(dev->device)) {
        ret = PTR_ERR(dev->device);
        goto destroy_class;
    }

    platform_set_drvdata(pdev, dev);

    pr_info("My UART driver loaded\n");
    return 0;

destroy_class:
    class_destroy(dev->class);
del_cdev:
    cdev_del(&dev->cdev);
unregister_region:
    unregister_chrdev_region(dev->devt, 1);
    return ret;
}

static void my_uart_remove(struct platform_device *pdev)
{
    struct my_uart *dev = platform_get_drvdata(pdev);

    device_destroy(dev->class, dev->devt);
    class_destroy(dev->class);
    cdev_del(&dev->cdev);
    unregister_chrdev_region(dev->devt, 1);

    if (!IS_ERR(dev->clk))
        clk_disable_unprepare(dev->clk);

    pr_info("My UART driver removed\n");
}

static const struct of_device_id my_uart_of_match[] = {
    { .compatible = "my,uart" },
    {}
};
MODULE_DEVICE_TABLE(of, my_uart_of_match);

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
MODULE_AUTHOR("Kalisetti Likitha");
MODULE_DESCRIPTION("Simple UART driver for Raspberry Pi 4");
