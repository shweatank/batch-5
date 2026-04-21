#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>

#define DEVICE_NAME "chardev"
#define CLASS_NAME  "chardev_class"
#define BUF_SIZE    1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Driver Dev");
MODULE_DESCRIPTION("Basic Character Device Driver");

static int    major;
static char   kbuf[BUF_SIZE];
static size_t kbuf_len;

static struct class  *dev_class;
static struct device *dev_device;
static struct cdev    chardev_cdev;
static dev_t          devno;

static int chardev_open(struct inode *inode, struct file *file)
{
    pr_info("chardev: device opened\n");
    return 0;
}

static int chardev_release(struct inode *inode, struct file *file)
{
    pr_info("chardev: device closed\n");
    return 0;
}

static ssize_t chardev_read(struct file *file, char __user *ubuf,
                             size_t len, loff_t *off)
{
    size_t to_copy;

    if (*off >= kbuf_len)
        return 0;

    to_copy = min(len, kbuf_len - (size_t)*off);

    if (copy_to_user(ubuf, kbuf + *off, to_copy))
        return -EFAULT;

    *off += to_copy;
    pr_info("chardev: read %zu bytes\n", to_copy);
    return to_copy;
}

static ssize_t chardev_write(struct file *file, const char __user *ubuf,
                              size_t len, loff_t *off)
{
    size_t to_copy = min(len, (size_t)(BUF_SIZE - 1));

    if (copy_from_user(kbuf, ubuf, to_copy))
        return -EFAULT;

    kbuf[to_copy] = '\0';
    kbuf_len = to_copy;
    pr_info("chardev: wrote %zu bytes\n", to_copy);
    return to_copy;
}

static const struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = chardev_open,
    .release = chardev_release,
    .read    = chardev_read,
    .write   = chardev_write,
};

static int __init chardev_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_err("chardev: failed to allocate major number\n");
        return ret;
    }
    major = MAJOR(devno);

    cdev_init(&chardev_cdev, &fops);
    chardev_cdev.owner = THIS_MODULE;

    ret = cdev_add(&chardev_cdev, devno, 1);
    if (ret < 0) {
        unregister_chrdev_region(devno, 1);
        pr_err("chardev: failed to add cdev\n");
        return ret;
    }

    dev_class = class_create(CLASS_NAME);
    if (IS_ERR(dev_class)) {
        cdev_del(&chardev_cdev);
        unregister_chrdev_region(devno, 1);
        return PTR_ERR(dev_class);
    }

    dev_device = device_create(dev_class, NULL, devno, NULL, DEVICE_NAME);
    if (IS_ERR(dev_device)) {
        class_destroy(dev_class);
        cdev_del(&chardev_cdev);
        unregister_chrdev_region(devno, 1);
        return PTR_ERR(dev_device);
    }

    pr_info("chardev: registered with major=%d\n", major);
    return 0;
}

static void __exit chardev_exit(void)
{
    device_destroy(dev_class, devno);
    class_destroy(dev_class);
    cdev_del(&chardev_cdev);
    unregister_chrdev_region(devno, 1);
    pr_info("chardev: unregistered\n");
}

module_init(chardev_init);
module_exit(chardev_exit);
