#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "ili9225"
#define CLASS_NAME "lcd"

static dev_t dev_num;
static struct cdev ili_cdev;
static struct class *ili_class;

static int ili_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "ILI9225 opened\n");
    return 0;
}

static int ili_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "ILI9225 closed\n");
    return 0;
}

static ssize_t ili_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
    uint8_t data[256];

    if (len > sizeof(data)) len = sizeof(data);

    if (copy_from_user(data, buf, len))
        return -EFAULT;

    /* Here you would call your SPI function to send data to the display */
    printk(KERN_INFO "ILI9225 write %zu bytes\n", len);

    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = ili_open,
    .release = ili_release,
    .write = ili_write,
};

static int __init ili_init(void)
{
    alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    cdev_init(&ili_cdev, &fops);
    cdev_add(&ili_cdev, dev_num, 1);

//    ili_class = class_create(THIS_MODULE, CLASS_NAME);
ili_class = class_create(CLASS_NAME);

    device_create(ili_class, NULL, dev_num, NULL, DEVICE_NAME);

    printk(KERN_INFO "ILI9225 char driver loaded\n");
    return 0;
}

static void __exit ili_exit(void)
{
    device_destroy(ili_class, dev_num);
    class_destroy(ili_class);
    cdev_del(&ili_cdev);
    unregister_chrdev_region(dev_num, 1);
    printk(KERN_INFO "ILI9225 char driver unloaded\n");
}

module_init(ili_init);
module_exit(ili_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Team4");
MODULE_DESCRIPTION("ILI9225 minimal char driver");

