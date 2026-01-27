#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>


#define DEVICE_NAME "calc_dev"
#define CLASS_NAME  "calc_class"

#define CALC_IOCTL _IOWR('k', 1, struct calc_data)

struct calc_data {
    int a;
    int b;
    char op;
    int result;
};

static int major;
static struct class *calc_class;
static struct device *calc_device;

static long calc_ioctl(struct file *file,
                       unsigned int cmd,
                       unsigned long arg)
{
    struct calc_data data;

    if (cmd != CALC_IOCTL)
        return -EINVAL;

    /* Copy data from user space */
    if (copy_from_user(&data, (struct calc_data *)arg,
                       sizeof(data)))
        return -EFAULT;

    /* Perform calculation */
    switch (data.op) {
    case '+': data.result = data.a + data.b; break;
    case '-': data.result = data.a - data.b; break;
    case '*': data.result = data.a * data.b; break;
    case '/':
        if (data.b == 0)
            return -EINVAL;
        data.result = data.a / data.b;
        break;
    default:
        return -EINVAL;
    }

    /* Copy result back to user space */
    if (copy_to_user((struct calc_data *)arg,
                     &data, sizeof(data)))
        return -EFAULT;

    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = calc_ioctl,
};

static int __init calc_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
   calc_class = class_create(CLASS_NAME);

    calc_device = device_create(calc_class, NULL,
                                MKDEV(major, 0),
                                NULL, DEVICE_NAME);

    printk(KERN_INFO "Calculator driver loaded\n");
    return 0;
}

static void __exit calc_exit(void)
{
    device_destroy(calc_class, MKDEV(major, 0));
    class_destroy(calc_class);
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Calculator driver unloaded\n");
}

module_init(calc_init);
module_exit(calc_exit);

MODULE_LICENSE("GPL");
