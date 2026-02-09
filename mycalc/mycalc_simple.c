#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "mycalc"
#define CLASS_NAME  "mycalcclass"
#define IOC_MAGIC   'M'

// Calculator request structure
struct calc_req {
    int a;
    int b;
    long result;
    int err; // 0 = OK, -EDOM = divide by zero
};

// IOCTL commands
#define CALC_ADD _IOWR(IOC_MAGIC, 1, struct calc_req)
#define CALC_SUB _IOWR(IOC_MAGIC, 2, struct calc_req)
#define CALC_MUL _IOWR(IOC_MAGIC, 3, struct calc_req)
#define CALC_DIV _IOWR(IOC_MAGIC, 4, struct calc_req)

// Device variables
static dev_t dev_num;
static struct cdev cdev;
static struct class *calc_class;
static struct device *calc_device;

// Perform calculation
static void do_calc(unsigned int cmd, struct calc_req *req) {
    req->err = 0;
    switch (cmd) {
        case CALC_ADD: req->result = (long)req->a + req->b; break;
        case CALC_SUB: req->result = (long)req->a - req->b; break;
        case CALC_MUL: req->result = (long)req->a * req->b; break;
        case CALC_DIV:
            if (req->b == 0) { req->result = 0; req->err = -EDOM; }
            else req->result = (long)req->a / req->b;
            break;
        default: req->result = 0; req->err = -ENOTTY; break;
    }
}

// IOCTL handler
static long calc_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    struct calc_req req;

    if (copy_from_user(&req, (void __user *)arg, sizeof(req)))
        return -EFAULT;

    do_calc(cmd, &req);

    if (copy_to_user((void __user *)arg, &req, sizeof(req)))
        return -EFAULT;

    return 0;
}

// File operations
static int calc_open(struct inode *inode, struct file *file) { return 0; }
static int calc_release(struct inode *inode, struct file *file) { return 0; }

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = calc_open,
    .release = calc_release,
    .unlocked_ioctl = calc_ioctl,
};

// Module initialization
static int __init calc_init(void) {
    int ret;

    // Allocate a device number
    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret) return ret;

    // Initialize cdev
    cdev_init(&cdev, &fops);
    cdev.owner = THIS_MODULE;
    cdev_add(&cdev, dev_num, 1);

    // Create class and device node
    calc_class = class_create(CLASS_NAME);
    if (IS_ERR(calc_class)) {
        cdev_del(&cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(calc_class);
    }

    calc_device = device_create(calc_class, NULL, dev_num, NULL, DEVICE_NAME);
    if (IS_ERR(calc_device)) {
        class_destroy(calc_class);
        cdev_del(&cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(calc_device);
    }

    pr_info("Simple calc driver loaded: /dev/%s major=%d\n",
            DEVICE_NAME, MAJOR(dev_num));
    return 0;
}

// Module exit
static void __exit calc_exit(void) {
    device_destroy(calc_class, dev_num);
    class_destroy(calc_class);
    cdev_del(&cdev);
    unregister_chrdev_region(dev_num, 1);
    pr_info("Simple calc driver unloaded\n");
}

module_init(calc_init);
module_exit(calc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhi");
MODULE_DESCRIPTION("Simple IOCTL Calculator Driver");

