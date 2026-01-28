#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DRV_NAME        "calc_ioctl"
#define DEV_NAME        "calc"
#define CLASS_NAME      "calcclass"

// ---------- IOCTL interface ----------
#define CALC_IOC_MAGIC  'C'

struct calc_req {
    int a;
    int b;
    long result;   // long to reduce overflow risk vs int (still possible)
    int err;       // 0 on success, -EINVAL/-ERANGE/-EDOM etc.
};

// Each ioctl copies struct calc_req in/out.
// User fills a,b ; driver fills result,err.

#define CALC_IOC_ADD  _IOWR(CALC_IOC_MAGIC, 1, struct calc_req)
#define CALC_IOC_SUB  _IOWR(CALC_IOC_MAGIC, 2, struct calc_req)
#define CALC_IOC_MUL  _IOWR(CALC_IOC_MAGIC, 3, struct calc_req)
#define CALC_IOC_DIV  _IOWR(CALC_IOC_MAGIC, 4, struct calc_req)
#define CALC_IOC_MOD  _IOWR(CALC_IOC_MAGIC, 5, struct calc_req)

// Optional: validate ioctl type
static inline bool calc_ioctl_valid(unsigned int cmd)
{
    return _IOC_TYPE(cmd) == CALC_IOC_MAGIC;
}

// ---------- Char device plumbing ----------
static dev_t g_dev;
static struct cdev g_cdev;
static struct class *g_class;
static struct device *g_device;

static long do_calc(unsigned int cmd, struct calc_req *r)
{
    r->err = 0;

    switch (cmd) {
    case CALC_IOC_ADD:
        r->result = (long)r->a + (long)r->b;
        
        pr_err(DRV_NAME "result:%ld\n", r->result);
	break;

    case CALC_IOC_SUB:
        r->result = (long)r->a - (long)r->b;
        break;

    case CALC_IOC_MUL:
        r->result = (long)r->a * (long)r->b;
        break;

    case CALC_IOC_DIV:
        if (r->b == 0) {
            r->err = -EDOM;      // domain error (divide by 0)
            r->result = 0;
        } else {
            r->result = (long)r->a / (long)r->b;
        }
        break;

    case CALC_IOC_MOD:
        if (r->b == 0) {
            r->err = -EDOM;
            r->result = 0;
        } else {
            r->result = (long)r->a % (long)r->b;
        }
        break;

    default:
        return -ENOTTY; // unknown ioctl
    }

    return 0;
}

static long calc_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    struct calc_req req;
    long ret;

    if (!calc_ioctl_valid(cmd))
        return -ENOTTY;

    // Copy request from user
    if (copy_from_user(&req, (void __user *)arg, sizeof(req)))
        return -EFAULT;
	
    ret = do_calc(cmd, &req);
    if (ret)
        return ret;

    // Copy response back to user (result + err updated)
    if (copy_to_user((void __user *)arg, &req, sizeof(req)))
        return -EFAULT;

    return 0;
}

static int calc_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int calc_release(struct inode *inode, struct file *file)
{
    return 0;
}

static const struct file_operations calc_fops = {
    .owner          = THIS_MODULE,
    .open           = calc_open,
    .release        = calc_release,
    .unlocked_ioctl = calc_unlocked_ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl   = calc_unlocked_ioctl,
#endif
};

static int __init calc_init(void)
{
    int ret;

    // Allocate a device number
    ret = alloc_chrdev_region(&g_dev, 0, 1, DRV_NAME);
    if (ret) {
        pr_err(DRV_NAME ": alloc_chrdev_region failed: %d\n", ret);
        return ret;
    }

    // Register cdev
    cdev_init(&g_cdev, &calc_fops);
    g_cdev.owner = THIS_MODULE;

    ret = cdev_add(&g_cdev, g_dev, 1);
    if (ret) {
        pr_err(DRV_NAME ": cdev_add failed: %d\n", ret);
        unregister_chrdev_region(g_dev, 1);
        return ret;
    }

    // Create class + device node via udev
    g_class = class_create(CLASS_NAME);
    if (IS_ERR(g_class)) {
        ret = PTR_ERR(g_class);
        pr_err(DRV_NAME ": class_create failed: %d\n", ret);
        cdev_del(&g_cdev);
        unregister_chrdev_region(g_dev, 1);
        return ret;
    }

    g_device = device_create(g_class, NULL, g_dev, NULL, DEV_NAME);
    if (IS_ERR(g_device)) {
        ret = PTR_ERR(g_device);
        pr_err(DRV_NAME ": device_create failed: %d\n", ret);
        class_destroy(g_class);
        cdev_del(&g_cdev);
        unregister_chrdev_region(g_dev, 1);
        return ret;
    }

    pr_info(DRV_NAME ": loaded. /dev/%s major=%d minor=%d\n",
            DEV_NAME, MAJOR(g_dev), MINOR(g_dev));
    return 0;
}

static void __exit calc_exit(void)
{
    device_destroy(g_class, g_dev);
    class_destroy(g_class);
    cdev_del(&g_cdev);
    unregister_chrdev_region(g_dev, 1);
    pr_info(DRV_NAME ": unloaded\n");
}

module_init(calc_init);
module_exit(calc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("IOCTL Calculator Driver");
