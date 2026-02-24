#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>       // register_chrdev
#include <linux/cdev.h>     // cdev structures
#include <linux/device.h>   // class_create / device_create
#include <linux/uaccess.h>  // copy_to_user / copy_from_user

#define DEVICE_NAME "mychardev"
#define BUF_LEN 80

static char msg[BUF_LEN];
static int msg_len;
static int major = 0; // Will hold major number

/* --- Modern cdev approach --- */
static dev_t dev;           // Encodes major/minor
static struct cdev my_cdev; // Kernel cdev structure
static struct class *mychar_class = NULL;
static struct device *mychar_device = NULL;

/* --- File operations --- */
static int dev_open(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "mychardev: Device opened\n");
    return 0;
}

static ssize_t dev_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset)
{
    if (copy_to_user(buffer, msg, msg_len))
        return -EFAULT;
    printk(KERN_INFO "mychardev: Sent %d bytes to user\n", msg_len);
    return msg_len;
}

static ssize_t dev_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset)
{
    if (len > BUF_LEN)
        len = BUF_LEN;

    if (copy_from_user(msg, buffer, len))
        return -EFAULT;

    msg_len = len;
    printk(KERN_INFO "mychardev: Received %ld bytes from user\n", len);
    return len;
}

static int dev_release(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "mychardev: Device closed\n");
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

/* --- 1️⃣ Simple Static Major Registration --- */
static int static_major_register(void)
{
    int ret = register_chrdev(240, DEVICE_NAME "_static", &fops);
    if (ret < 0) {
        printk(KERN_ALERT "Failed to register static major 240\n");
        return ret;
    }
    printk(KERN_INFO "Static major device registered: 240\n");
    return 0;
}

/* --- 2️⃣ Dynamic Major Registration --- */
static int dynamic_major_register(void)
{
    major = register_chrdev(0, DEVICE_NAME "_dynamic", &fops);
    if (major < 0) {
        printk(KERN_ALERT "Failed to register dynamic major\n");
        return major;
    }
    printk(KERN_INFO "Dynamic major device registered: %d\n", major);
    printk(KERN_INFO "You can create node with: sudo mknod /dev/%s c %d 0\n",
           DEVICE_NAME "_dynamic", major);
    return 0;
}

/* --- 3️⃣ Modern cdev + class_create + device_create --- */
static int modern_cdev_register(void)
{
    int ret;

    /* Allocate dynamic major/minor */
    ret = alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME "_modern");
    if (ret < 0) {
        printk(KERN_ALERT "Failed to allocate chrdev region\n");
        return ret;
    }
    printk(KERN_INFO "Modern device registered with major %d minor %d\n",
           MAJOR(dev), MINOR(dev));

    /* Initialize cdev */
    cdev_init(&my_cdev, &fops);
    my_cdev.owner = THIS_MODULE;

    /* Add to kernel */
    ret = cdev_add(&my_cdev, dev, 1);
    if (ret < 0) {
        unregister_chrdev_region(dev, 1);
        printk(KERN_ALERT "Failed to add cdev\n");
        return ret;
    }

    /* Create class and device node in /dev automatically */
    mychar_class = class_create(DEVICE_NAME "_class");
    if (IS_ERR(mychar_class)) {
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev, 1);
        return PTR_ERR(mychar_class);
    }

    mychar_device = device_create(mychar_class, NULL, dev, NULL, DEVICE_NAME "_modern");
    if (IS_ERR(mychar_device)) {
        class_destroy(mychar_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev, 1);
        return PTR_ERR(mychar_device);
    }

    printk(KERN_INFO "Device node /dev/%s created automatically\n", DEVICE_NAME "_modern");
    return 0;
}

/* --- Module Init --- */
static int __init chardev_init(void)
{
    int ret;

    printk(KERN_INFO "--- Loading mychardev module ---\n");

    ret = static_major_register();   // optional, demonstrates static major
    if (ret < 0) return ret;

    //ret = dynamic_major_register();  // optional, demonstrates dynamic major
    //if (ret < 0) return ret;

    //ret = modern_cdev_register();    // preferred modern way
    //if (ret < 0) return ret;

    return 0;
}

/* --- Module Exit --- */
static void __exit chardev_exit(void)
{
    printk(KERN_INFO "--- Unloading mychardev module ---\n");

    /* Cleanup modern device */
    device_destroy(mychar_class, dev);
    class_destroy(mychar_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev, 1);

    /* Cleanup dynamic major device */
    unregister_chrdev(major, DEVICE_NAME "_dynamic");

    /* Cleanup static major device */
    unregister_chrdev(240, DEVICE_NAME "_static");

    printk(KERN_INFO "All devices unregistered\n");
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Character Device Demo: static, dynamic, modern cdev");

