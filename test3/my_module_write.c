#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // copy_from_user
#include <linux/cdev.h>

#define DEVICE_NAME "mydevice"

struct my_data {
    int id;
    char name[20];
};

static int major;
static struct cdev my_cdev;

static ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
    struct my_data data;

    if (len != sizeof(data)) {
        printk(KERN_WARNING "Invalid data size\n");
        return -EINVAL;
    }

    if (copy_from_user(&data, buf, sizeof(data))) {
        return -EFAULT;
    }

    printk(KERN_INFO "Received data: id=%d, name=%s\n", data.id, data.name);

    return len; 
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = my_write,
};

static int __init my_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "Failed to register device\n");
        return major;
    }

    printk(KERN_INFO "Device registered with major %d\n", major);
    return 0;
}

static void __exit my_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Device unregistered\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhi");
MODULE_DESCRIPTION("Example module to receive struct from user via write");

