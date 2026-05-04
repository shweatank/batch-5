// SPDX-License-Identifier: GPL-2.0
/*
 * basic_char_driver: Production-style Linux char device driver for 6.8+
 * Features: open, release, read, write, unlocked_ioctl, llseek, poll, mutex, error handling, logging
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/ioctl.h>

#define DRIVER_NAME     "basic_char"
#define DEVICE_NAME     "basic_char"
#define CLASS_NAME      "basic_char"
#define BUF_SIZE        4096

/* IOCTL definitions */
#define BASIC_CHAR_MAGIC      'B'
#define IOCTL_CLEAR_BUFFER    _IO(BASIC_CHAR_MAGIC, 0)
#define IOCTL_GET_BUFLEN      _IOR(BASIC_CHAR_MAGIC, 1, int)

static dev_t dev_num;
static struct cdev basic_cdev;
static struct class *basic_class;
static struct device *basic_device;

static char *device_buffer;
static size_t buffer_size;
static struct mutex buffer_mutex;
static wait_queue_head_t wq;
static int data_available;

static int basic_open(struct inode *inode, struct file *file)
{
    pr_info(DRIVER_NAME ": device opened\n");
    return 0;
}

static int basic_release(struct inode *inode, struct file *file)
{
    pr_info(DRIVER_NAME ": device closed\n");
    return 0;
}

static ssize_t basic_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    ssize_t ret = 0;

    if (mutex_lock_interruptible(&buffer_mutex))
        return -ERESTARTSYS;

    if (*ppos >= buffer_size) {
        ret = 0;
        goto out;
    }

    if (count > buffer_size - *ppos)
        count = buffer_size - *ppos;

    if (copy_to_user(buf, device_buffer + *ppos, count)) {
        ret = -EFAULT;
        goto out;
    }

    *ppos += count;
    ret = count;
    data_available = 0;

out:
    mutex_unlock(&buffer_mutex);
    return ret;
}

static ssize_t basic_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    ssize_t ret = 0;

    if (mutex_lock_interruptible(&buffer_mutex))
        return -ERESTARTSYS;

    if (count > BUF_SIZE)
        count = BUF_SIZE;

    if (copy_from_user(device_buffer, buf, count)) {
        ret = -EFAULT;
        goto out;
    }

    buffer_size = count;
    *ppos = 0;
    data_available = 1;
    wake_up_interruptible(&wq);

    ret = count;

out:
    mutex_unlock(&buffer_mutex);
    return ret;
}

static long basic_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int len;

    if (mutex_lock_interruptible(&buffer_mutex))
        return -ERESTARTSYS;

    switch (cmd) {
    case IOCTL_CLEAR_BUFFER:
        memset(device_buffer, 0, BUF_SIZE);
        buffer_size = 0;
        data_available = 0;
        pr_info(DRIVER_NAME ": buffer cleared via ioctl\n");
        break;
    case IOCTL_GET_BUFLEN:
        len = buffer_size;
        if (copy_to_user((int __user *)arg, &len, sizeof(int))) {
            mutex_unlock(&buffer_mutex);
            return -EFAULT;
        }
        break;
    default:
        mutex_unlock(&buffer_mutex);
        return -ENOTTY;
    }

    mutex_unlock(&buffer_mutex);
    return 0;
}

static loff_t basic_llseek(struct file *file, loff_t offset, int whence)
{
    loff_t newpos = 0;

    mutex_lock(&buffer_mutex);
    switch (whence) {
    case SEEK_SET:
        newpos = offset;
        break;
    case SEEK_CUR:
        newpos = file->f_pos + offset;
        break;
    case SEEK_END:
        newpos = buffer_size + offset;
		break;
    default:
        mutex_unlock(&buffer_mutex);
        return -EINVAL;
    }
    if (newpos < 0 || newpos > buffer_size) {
        mutex_unlock(&buffer_mutex);
        return -EINVAL;
    }
    file->f_pos = newpos;
    mutex_unlock(&buffer_mutex);
    return newpos;
}

static __poll_t basic_poll(struct file *file, struct poll_table_struct *wait)
{
    __poll_t mask = 0;

    poll_wait(file, &wq, wait);

    mutex_lock(&buffer_mutex);
    if (buffer_size > 0)
        mask |= EPOLLIN | POLLIN | POLLRDNORM;
    if (buffer_size < BUF_SIZE)
        mask |= EPOLLOUT | POLLOUT | POLLWRNORM;
    mutex_unlock(&buffer_mutex);

    return mask;
}

static const struct file_operations basic_fops = {
    .owner          = THIS_MODULE,
    .open           = basic_open,
    .release        = basic_release,
    .read           = basic_read,
    .write          = basic_write,
    .unlocked_ioctl = basic_ioctl,
    .llseek         = basic_llseek,
    .poll           = basic_poll,
};

static int __init basic_init(void)
{
    int ret;

    pr_info(DRIVER_NAME ": initializing\n");

    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret) {
        pr_err(DRIVER_NAME ": alloc_chrdev_region failed\n");
        return ret;
    }

    cdev_init(&basic_cdev, &basic_fops);
    basic_cdev.owner = THIS_MODULE;

    ret = cdev_add(&basic_cdev, dev_num, 1);
    if (ret) {
        pr_err(DRIVER_NAME ": cdev_add failed\n");
        goto unregister_chrdev;
    }

    basic_class = class_create(CLASS_NAME);
    if (IS_ERR(basic_class)) {
        pr_err(DRIVER_NAME ": class_create failed\n");
        ret = PTR_ERR(basic_class);
        goto del_cdev;
    }

    basic_device = device_create(basic_class, NULL, dev_num, NULL, DEVICE_NAME);
    if (IS_ERR(basic_device)) {
        pr_err(DRIVER_NAME ": device_create failed\n");
        ret = PTR_ERR(basic_device);
        goto destroy_class;
    }

    device_buffer = kzalloc(BUF_SIZE, GFP_KERNEL);
    if (!device_buffer) {
        pr_err(DRIVER_NAME ": buffer allocation failed\n");
        ret = -ENOMEM;
        goto destroy_device;
    }
    buffer_size = 0;
    mutex_init(&buffer_mutex);
    init_waitqueue_head(&wq);
    data_available = 0;

    pr_info(DRIVER_NAME ": loaded successfully, major=%d\n", MAJOR(dev_num));
    return 0;

destroy_device:
    device_destroy(basic_class, dev_num);
destroy_class:
    class_destroy(basic_class);
del_cdev:
    cdev_del(&basic_cdev);
unregister_chrdev:
    unregister_chrdev_region(dev_num, 1);
    return ret;
}

static void __exit basic_exit(void)
{
    kfree(device_buffer);
    device_destroy(basic_class, dev_num);
    class_destroy(basic_class);
    cdev_del(&basic_cdev);
    unregister_chrdev_region(dev_num, 1);
    pr_info(DRIVER_NAME ": unloaded\n");
}

module_init(basic_init);
module_exit(basic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Linux Kernel Maintainer");
MODULE_DESCRIPTION("Production-style char device driver for Linux 6.8+");
MODULE_VERSION("1.0");
