// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "basic_drv"
#define BUF_SIZE 256

static dev_t dev_num;
static struct cdev basic_cdev;
static struct class *basic_class;
static char device_buffer[BUF_SIZE];
static size_t buffer_size;

static int basic_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int basic_release(struct inode *inode, struct file *file)
{
    return 0;
}

static ssize_t basic_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    ssize_t ret;

    if (*ppos >= buffer_size)
        return 0;

    if (count > buffer_size - *ppos)
        count = buffer_size - *ppos;

    if (copy_to_user(buf, device_buffer + *ppos, count))
        return -EFAULT;

    *ppos += count;
    ret = count;
    return ret;
}

static ssize_t basic_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    if (count > BUF_SIZE)
        count = BUF_SIZE;

    if (copy_from_user(device_buffer, buf, count))
        return -EFAULT;

    buffer_size = count;
    return count;
}

static const struct file_operations basic_fops = {
    .owner = THIS_MODULE,
    .open = basic_open,
    .release = basic_release,
    .read = basic_read,
    .write = basic_write,
};

static int __init basic_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret)
        return ret;

    cdev_init(&basic_cdev, &basic_fops);
    basic_cdev.owner = THIS_MODULE;

    ret = cdev_add(&basic_cdev, dev_num, 1);
    if (ret)
        goto unregister_region;

    basic_class = class_create(DEVICE_NAME);
    if (IS_ERR(basic_class)) {
        ret = PTR_ERR(basic_class);
        goto del_cdev;
    }

    if (!device_create(basic_class, NULL, dev_num, NULL, DEVICE_NAME)) {
        ret = -ENOMEM;
        goto destroy_class;
    }

    pr_info("basic_drv: loaded\n");
    return 0;

destroy_class:
    class_destroy(basic_class);
del_cdev:
    cdev_del(&basic_cdev);
unregister_region:
    unregister_chrdev_region(dev_num, 1);
    return ret;
}

static void __exit basic_exit(void)
{
    device_destroy(basic_class, dev_num);
    class_destroy(basic_class);
    cdev_del(&basic_cdev);
    unregister_chrdev_region(dev_num, 1);
    pr_info("basic_drv: unloaded\n");
}

module_init(basic_init);
module_exit(basic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Linux Kernel Developer");
MODULE_DESCRIPTION("Very basic char device driver for /dev/basic_drv");
