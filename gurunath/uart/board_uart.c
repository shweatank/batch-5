#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/serial_core.h>
#include <linux/serial_reg.h>

#define DEVICE_NAME "my_uart"
#define BUF_LEN 256

static int major;
static char msg[BUF_LEN];
static int msg_len;

static int dev_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "UART: Device opened\n");
    return 0;
}

static int dev_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "UART: Device closed\n");
    return 0;
}

static ssize_t dev_read(struct file *file, char __user *buffer, size_t len, loff_t *offset) {
    int bytes_read = msg_len;
    if (copy_to_user(buffer, msg, msg_len))
        return -EFAULT;

    pr_info("in read:\n");

    printk(KERN_INFO "UART: Read %d bytes\n", bytes_read);
    return bytes_read;
}

static ssize_t dev_write(struct file *file, const char __user *buffer, size_t len, loff_t *offset) {
    if (len > BUF_LEN) len = BUF_LEN;
    if (copy_from_user(msg, buffer, len))
        return -EFAULT;

    pr_info("in write:\n");

    msg_len = len;
    printk(KERN_INFO "UART: Written %zu bytes\n", len);
    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_release,
    .read = dev_read,
    .write = dev_write,
};

static int __init uart_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "UART: Registering char device failed\n");
        return major;
    }
    printk(KERN_INFO "UART: Registered with major number %d\n", major);
    return 0;
}

static void __exit uart_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "UART: Unregistered\n");
}

module_init(uart_init);
module_exit(uart_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ChatGPT");
MODULE_DESCRIPTION("Simple UART LDD for Raspberry Pi 4");

