#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "square_dev"
#define BUF_LEN 64


static int major_num;
static char result_buf[BUF_LEN];
static int result_len;


static int device_open(struct inode *inode, struct file *file) {
    pr_info("square_dev: Device opened\n");
    return 0;
}


static int device_release(struct inode *inode, struct file *file) {
    pr_info("square_dev: Device closed\n");
    return 0;
}


static ssize_t device_write(struct file *filp, const char __user *buffer, size_t len, loff_t *off) {
    int num;
    long long square; 

    if (len != sizeof(int)) {
        pr_info("square_dev: write expects %zu bytes (sizeof(int)), got %zu\n", sizeof(int), len);
        return -EINVAL;
    }

    if (copy_from_user(&num, buffer, sizeof(int))) {
        return -EFAULT;
    }

    square = (long long)num * (long long)num;

 
    result_len = snprintf(result_buf, BUF_LEN, "%lld", square);
    pr_info("square_dev: %d^2 = %lld\n", num, square);

    return sizeof(int);
}


static ssize_t device_read(struct file *filp, char __user *buffer, size_t len, loff_t *off) {
    ssize_t bytes_to_copy;

    if (*off >= result_len)
        return 0;

    bytes_to_copy = result_len - *off;
    if (len < bytes_to_copy)
        bytes_to_copy = len;

    if (copy_to_user(buffer, result_buf + *off, bytes_to_copy))
        return -EFAULT;

    *off += bytes_to_copy;
    return bytes_to_copy;
}


static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_release,
    .write = device_write,
    .read = device_read,
};

static int __init square_init(void) {
    major_num = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_num < 0) {
        pr_info("square_dev: failed to register character device\n");
        return major_num;
    }

    pr_info("square_dev: registered with major number %d\n", major_num);
    pr_info("square_dev: create device node with: mknod /dev/%s c %d 0", DEVICE_NAME, major_num);
    return 0;
}


static void __exit square_exit(void) {
    unregister_chrdev(major_num, DEVICE_NAME);
    pr_info("square_dev: unregistered device\n");
}

module_init(square_init);
module_exit(square_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhi");
MODULE_DESCRIPTION("Simple Square Character Driver");


