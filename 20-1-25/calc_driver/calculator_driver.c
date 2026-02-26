#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "calc_dev"
#define BUF_LEN 256

static int major;
static char kbuf[BUF_LEN];
static int result;
static int data_ready;

/* Perform calculation */
static int calculate(int a, int b, char op)
{
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (b == 0)
            return 0;
        return a / b;
    default:
        return 0;
    }
}

/* Write from user */
static ssize_t dev_write(struct file *file,
                         const char __user *buf,
                         size_t len,
                         loff_t *off)
{
    int a, b;
    char op;

    if (len > BUF_LEN)
        len = BUF_LEN;

    if (copy_from_user(kbuf, buf, len))
        return -EFAULT;

    kbuf[len] = '\0';

    /* Parse input */
    if (sscanf(kbuf, "%d %c %d", &a, &op, &b) != 3)
        return -EINVAL;

    result = calculate(a, b, op);
    data_ready = 1;

    return len;
}

/* Read to user */
static ssize_t dev_read(struct file *file,
                        char __user *buf,
                        size_t len,
                        loff_t *off)
{
    int ret;
    char out[64];

    if (!data_ready)
        return 0;

    ret = snprintf(out, sizeof(out), "Result: %d\n", result);

    if (*off >= ret)
        return 0;

    if (copy_to_user(buf, out, ret))
        return -EFAULT;

    *off = ret;
    data_ready = 0;
    return ret;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = dev_read,
    .write = dev_write,
};

static int __init calc_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    printk(KERN_INFO "Calculator driver loaded. Major = %d\n", major);
    return 0;
}

static void __exit calc_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Calculator driver unloaded\n");
}

module_init(calc_init);
module_exit(calc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sneha");
MODULE_DESCRIPTION("Simple calculator char driver");

