#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "techdhaba_char"

static int major;

/* File operations */
static int dev_open(struct inode *inode, struct file *file)
{
    pr_info("techdhaba_char: device opened\n");
    return 0;
}

static int dev_release(struct inode *inode, struct file *file)
{
    pr_info("techdhaba_char: device closed\n");
    return 0;
}

static ssize_t dev_read(struct file *file, char __user *buf,
                        size_t len, loff_t *offset)
{
    char msg[] = "Hello from TechDhaba driver\n";
    size_t msg_len = strlen(msg);

    if (*offset >= msg_len)
        return 0;

    if (copy_to_user(buf, msg, msg_len))
        return -EFAULT;

    *offset = msg_len;
    return msg_len;
}

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = dev_open,
    .release = dev_release,
    .read    = dev_read,
};

/* Module init */
static int __init techdhaba_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        pr_err("techdhaba_char: failed to register\n");
        return major;
    }

    pr_info("techdhaba_char loaded, major=%d\n", major);
    return 0;
}

/* Module exit */
static void __exit techdhaba_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    pr_info("techdhaba_char unloaded\n");
}

module_init(techdhaba_init);
module_exit(techdhaba_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Simple char driver loaded using modprobe");
