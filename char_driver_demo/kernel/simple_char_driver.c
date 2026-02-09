#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "simple_char"
#define BUFFER_SIZE 1024

static int major;
static char kernel_buffer[BUFFER_SIZE];
static int buffer_len = 0;

/* Function declarations */
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char __user *, size_t, loff_t *);

static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

/* Open */
static int dev_open(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "simple_char: open() called\n");
    return 0;
}

/* Read */
static ssize_t dev_read(struct file *filep, char __user *buffer,
                        size_t len, loff_t *offset)
{
    int bytes;

    if (*offset >= buffer_len)
        return 0;

    bytes = min(len, (size_t)(buffer_len - *offset));

    if (copy_to_user(buffer, kernel_buffer + *offset, bytes))
        return -EFAULT;

    *offset += bytes;
    printk(KERN_INFO "simple_char: read() called\n");
    return bytes;
}

/* Write */
static ssize_t dev_write(struct file *filep, const char __user *buffer,
                         size_t len, loff_t *offset)
{
    int bytes = min(len, (size_t)BUFFER_SIZE);

    if (copy_from_user(kernel_buffer, buffer, bytes))
        return -EFAULT;

    buffer_len = bytes;
    printk(KERN_INFO "simple_char: write() called\n");
    return bytes;
}

/* Close */
static int dev_release(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "simple_char: close() called\n");
    return 0;
}

/* Init */
static int __init simple_char_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "simple_char: Registration failed\n");
        return major;
    }

    printk(KERN_INFO "simple_char: Registered with major number %d\n", major);
    return 0;
}

/* Exit */
static void __exit simple_char_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "simple_char: Unregistered device\n");
}

module_init(simple_char_init);
module_exit(simple_char_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ABHI");
MODULE_DESCRIPTION("Simple Char Driver");
