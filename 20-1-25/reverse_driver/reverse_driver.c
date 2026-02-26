#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "reverse_dev"
#define BUF_LEN 256

static int major;
static char kbuf[BUF_LEN];
static int data_size;

/* Reverse string function */
static void reverse_string(char *str, int len)
{
    int i;
    char temp;
    for (i = 0; i < len / 2; i++) {
        temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

/* Write from user */
static ssize_t dev_write(struct file *file,
                         const char __user *buf,
                         size_t len,
                         loff_t *off)
{
    if (len > BUF_LEN)
        len = BUF_LEN;

    if (copy_from_user(kbuf, buf, len))
        return -EFAULT;

    data_size = len;
    reverse_string(kbuf, data_size);

    return len;
}

/* Read to user */
static ssize_t dev_read(struct file *file,
                        char __user *buf,
                        size_t len,
                        loff_t *off)
{
    if (*off >= data_size)
        return 0;

    if (copy_to_user(buf, kbuf, data_size))
        return -EFAULT;

    *off = data_size;
    return data_size;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = dev_read,
    .write = dev_write,
};

static int __init reverse_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    printk(KERN_INFO "Reverse driver loaded. Major = %d\n", major);
    return 0;
}

static void __exit reverse_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Reverse driver unloaded\n");
}

module_init(reverse_init);
module_exit(reverse_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sneha");
MODULE_DESCRIPTION("Reverse string char driver");

