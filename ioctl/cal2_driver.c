#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include<linux/slab.h>

#define DEVICE_NAME "basic_ioctl"
#define IOCTL_MAGIC 'B'

struct st {
    int a;
    int b;
    int res;
};

#define IOCTL_ADD _IOWR(IOCTL_MAGIC, 1, struct st)
#define IOCTL_SUB _IOWR(IOCTL_MAGIC, 2, struct st)
#define IOCTL_MUL _IOWR(IOCTL_MAGIC, 3, struct st)
#define IOCTL_DIV _IOWR(IOCTL_MAGIC, 4, struct st)

static int major;

/* ioctl handler */
static long basic_ioctl(struct file *file,
                        unsigned int cmd,
                        unsigned long arg)
{
    struct st *val;
val=kmalloc(sizeof(struct st),GFP_KERNEL);

    if (copy_from_user(val, (struct st __user *)arg, sizeof(struct st)))
        return -EFAULT;

    printk(KERN_INFO "Kernel received %d and %d\n", val->a, val->b);

    switch (cmd) {
    case IOCTL_ADD:
        val->res = val->a + val->b;
        break;

    case IOCTL_SUB:
        val->res = val->a - val->b;
        break;

    case IOCTL_MUL:
        val->res = val->a * val->b;
        break;

    case IOCTL_DIV:
        if (val->b == 0)
            return -EINVAL;
        val->res = val->a / val->b;
        break;

    default:
        return -ENOTTY;
    }

    if (copy_to_user((struct st __user *)arg, val, sizeof(struct st)))
        return -EFAULT;

    return 0;
kfree(val);
}

static struct file_operations fops = {
    .owner          = THIS_MODULE,
    .unlocked_ioctl = basic_ioctl,
};

static int __init basic_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    pr_info("basic_ioctl loaded, major=%d\n", major);
    return 0;
}

static void __exit basic_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    pr_info("basic_ioctl unloaded\n");
}

module_init(basic_init);
module_exit(basic_exit);

MODULE_LICENSE("GPL");

