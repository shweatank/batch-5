#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "kcalc"
#define BUF_SIZE 128

static int major;
static char kbuf[BUF_SIZE];
static int result;
static int data_ready = 0;


static ssize_t kcalc_write(struct file *file,
                           const char __user *buf,
                           size_t len,
                           loff_t *off)
{
    char op;
    int a, b;

    if (len >= BUF_SIZE)
        return -EINVAL;

    if (copy_from_user(kbuf, buf, len))
        return -EFAULT;

    kbuf[len] = '\0';

    if (sscanf(kbuf, "%d %d %c", &a, &b, &op) != 3)
        return -EINVAL;

    switch (op) {
    case '+': result = a + b; break;
    case '-': result = a - b; break;
    case '*': result = a * b; break;
    case '/':
        if (b == 0) {
            printk(KERN_INFO "kcalc: Division by zero\n");
            return -EINVAL;
        }
        result = a / b;
        break;
    default:
        return -EINVAL;
    }

    data_ready = 1;
    printk(KERN_INFO "kcalc: %d %c %d = %d\n", a, op, b, result);

    return len;
}

static ssize_t kcalc_read(struct file *file,
                          char __user *buf,
                          size_t len,
                          loff_t *off)
{
    char out[BUF_SIZE];
    int out_len;

    if (!data_ready)
        return 0;

    out_len = snprintf(out, BUF_SIZE, "Result = %d\n", result);

    if (*off >= out_len)
        return 0;

    if (copy_to_user(buf, out, out_len))
        return -EFAULT;

    *off += out_len;
    data_ready = 0;

    return out_len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = kcalc_read,
    .write = kcalc_write,
};

static int __init kcalc_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0)
        return major;

    printk(KERN_INFO "kcalc loaded: major=%d\n", major);
    printk(KERN_INFO "Create device: mknod /dev/kcalc c %d 0\n", major);
    return 0;
}

static void __exit kcalc_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "kcalc unloaded\n");
}

module_init(kcalc_init);
module_exit(kcalc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhi");
MODULE_DESCRIPTION("User-kernel calculator using character device");

