#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define UART_DR  0x00
#define UART_FR  0x18
#define FR_TXFF  (1 << 5)
#define FR_RXFE  (1 << 4)

struct my_uart {
    void __iomem *base;
    struct cdev cdev;
    dev_t devt;
};

static void uart_send(struct my_uart *dev, char c)
{
    while (readl(dev->base + UART_FR) & FR_TXFF)
        cpu_relax();
    writel(c, dev->base + UART_DR);
}

static char uart_recv(struct my_uart *dev)
{
    if (readl(dev->base + UART_FR) & FR_RXFE)
        return 0;
    return readl(dev->base + UART_DR);
}

static ssize_t my_write(struct file *file, const char __user *buf,
                        size_t len, loff_t *off)
{
    struct my_uart *dev = file->private_data;
    char ker_buff[128];
    int i;

    if (len > sizeof(ker_buff))
        len = sizeof(ker_buff);

    if (copy_from_user(ker_buff, buf, len))
        return -EFAULT;

    for (i = 0; i < len; i++)
        uart_send(dev, ker_buff[i]);

    return len;
}

static ssize_t my_read(struct file *file, char __user *buf,
                       size_t len, loff_t *off)
{
    struct my_uart *dev = file->private_data;
    char c;

    c = uart_recv(dev);

    if (copy_to_user(buf, &c, 1))
        return -EFAULT;

    return 1;
}

static int my_open(struct inode *inode, struct file *file)
{
    struct my_uart *dev = container_of(inode->i_cdev, struct my_uart, cdev);
    file->private_data = dev;
    return 0;
}

static const struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .read  = my_read,
    .write = my_write,
    .open  = my_open,
};

static int my_uart_probe(struct platform_device *pdev)
{
    struct resource *res;
    struct my_uart *dev;
    int ret;

    dev = devm_kzalloc(&pdev->dev, sizeof(*dev), GFP_KERNEL);
    if (!dev)
        return -ENOMEM;

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    dev->base = devm_ioremap_resource(&pdev->dev, res);
    if (IS_ERR(dev->base))
        return PTR_ERR(dev->base);

    /* Allocate a character device number */
    ret = alloc_chrdev_region(&dev->devt, 0, 1, "my_uart");
    if (ret) {
        pr_err("Failed to allocate char dev\n");
        return ret;
    }

    cdev_init(&dev->cdev, &my_fops);
    dev->cdev.owner = THIS_MODULE;
    ret = cdev_add(&dev->cdev, dev->devt, 1);
    if (ret) {
        unregister_chrdev_region(dev->devt, 1);
        pr_err("Failed to add cdev\n");
        return ret;
    }

    platform_set_drvdata(pdev, dev);
    pr_info("My UART driver probed, major=%d, minor=%d\n",
            MAJOR(dev->devt), MINOR(dev->devt));

    return 0;
}

static int my_uart_remove(struct platform_device *pdev)
{
    struct my_uart *dev = platform_get_drvdata(pdev);

    cdev_del(&dev->cdev);
    unregister_chrdev_region(dev->devt, 1);

    pr_info("My UART driver removed\n");
    return 0;
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
MODULE_AUTHOR("KISHORE");
MODULE_DESCRIPTION("Simple UART driver using Device Tree");

