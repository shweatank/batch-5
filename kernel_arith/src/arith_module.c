#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "arith"
#define BUF_LEN 128

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhi");
MODULE_DESCRIPTION("Simple arithmetic kernel module (integer only)");

static int major;
static char msg[BUF_LEN];
static size_t msg_len = 0;

// File operations prototypes
static ssize_t device_read(struct file *file, char __user *buf, size_t len, loff_t *offset);
static ssize_t device_write(struct file *file, const char __user *buf, size_t len, loff_t *offset);

struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = device_read,
    .write = device_write,
};

static ssize_t device_write(struct file *file, const char __user *buf, size_t len, loff_t *offset)
{
    int nums[2];
    int sum, diff, prod;
    int quot_int = 0;   // quotient multiplied by 100 for 2 decimal precision

    if (len > sizeof(nums)) len = sizeof(nums);

    if (copy_from_user(nums, buf, len))
        return -EFAULT;

    sum = nums[0] + nums[1];
    diff = nums[0] - nums[1];
    prod = nums[0] * nums[1];

    if (nums[1] != 0)
        quot_int = (nums[0] * 100) / nums[1]; // store quotient*100

    msg_len = snprintf(msg, BUF_LEN,
                       "Sum=%d Diff=%d Prod=%d Quot=%d.%02d\n",
                       sum, diff, prod, quot_int / 100, quot_int % 100);

    return len;
}

static ssize_t device_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
    if (*offset >= msg_len)
        return 0; // EOF

    if (len > msg_len - *offset)
        len = msg_len - *offset;

    if (copy_to_user(buf, msg + *offset, len))
        return -EFAULT;

    *offset += len;
    return len;
}

static int __init arith_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "Failed to register char device: %d\n", major);
        return major;
    }

    printk(KERN_INFO "Arithmetic module loaded with major %d\n", major);
    return 0;
}

static void __exit arith_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Arithmetic module unloaded\n");
}

module_init(arith_init);
module_exit(arith_exit);

