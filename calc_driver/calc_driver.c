#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "calc_dev"
#define BUF_LEN 128

static int major_num;
static char msg[BUF_LEN];
static int msg_len;

typedef enum { ADD=0, SUB=1 } operation_t;

struct calc_data {
    int num1;
    int num2;
    operation_t op;
};

static int device_open(struct inode *inode, struct file *file) {
    pr_info("Calculator Device Opened\n");
    return 0;
}

static int device_release(struct inode *inode, struct file *file) {
    pr_info( "Calculator Device Closed\n");
    return 0;
}

static ssize_t device_write(struct file *filp, const char __user *buffer, size_t len, loff_t *off) {
    struct calc_data data;
    int result;

    if (len != sizeof(struct calc_data))
        return -EINVAL;

    if (copy_from_user(&data, buffer, sizeof(struct calc_data)))
        return -EFAULT;

    switch (data.op) {
        case ADD: result = data.num1 + data.num2; break;
        case SUB: result = data.num1 - data.num2; break;
        default: return -EINVAL;
    }

    msg_len = snprintf(msg, BUF_LEN, "%d", result);
    pr_info("Calc: %d %s %d = %d\n", data.num1, (data.op==ADD)?"ADD":"SUB", data.num2, result);

    return len;
}

static ssize_t device_read(struct file *filp, char __user *buffer, size_t len, loff_t *off) {
    if (*off >= msg_len) return 0;

    if (len > msg_len - *off) len = msg_len - *off;

    if (copy_to_user(buffer, msg + *off, len)) return -EFAULT;

    *off += len;
    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_release,
    .write = device_write,
    .read = device_read
};

static int __init calc_init(void) {
    major_num = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_num < 0) {
        pr_err("Failed to register device\n");
        return major_num;
    }
    pr_info("Calculator Device registered with major number %d\n", major_num);
    pr_info("mknod /dev/%s c %d 0\n", DEVICE_NAME, major_num);
    return 0;
}

static void __exit calc_exit(void) {
    unregister_chrdev(major_num, DEVICE_NAME);
    pr_info( "Calculator Device unregistered\n");
}

module_init(calc_init);
module_exit(calc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhi");
MODULE_DESCRIPTION("Simple Calculator Character Driver");
