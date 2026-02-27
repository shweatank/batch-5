#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/suspend.h>

/* ============================================================
 * Module Parameter (runtime configurable)
 * ============================================================ */

static int pm_level = 0;
/*
 * pm_level:
 * 0 = Normal operation
 * 1 = Idle-like (light suspend)
 * 2 = Power-down-like (deep suspend)
 */
module_param(pm_level, int, 0644);
MODULE_PARM_DESC(pm_level, "Power level: 0=normal, 1=idle, 2=deep");

/* ============================================================
 * Character Device Definitions
 * ============================================================ */

#define DEV_NAME "pm_demo"

static dev_t dev_num;
static struct cdev pm_cdev;
static struct class *pm_class;

/* ============================================================
 * File Operations
 * ============================================================ */

static int pm_open(struct inode *inode, struct file *file)
{
    pr_info("pm_demo: device opened\n");
    return 0;
}

static int pm_release(struct inode *inode, struct file *file)
{
    pr_info("pm_demo: device closed\n");
    return 0;
}

static ssize_t pm_read(struct file *file,
                       char __user *buf,
                       size_t len,
                       loff_t *offset)
{
    char msg[64];
    int size;

    size = snprintf(msg, sizeof(msg),
                    "pm_level=%d\n", pm_level);

    return simple_read_from_buffer(buf, len, offset, msg, size);
}

static const struct file_operations pm_fops = {
    .owner   = THIS_MODULE,
    .open    = pm_open,
    .release = pm_release,
    .read    = pm_read,
};

/* ============================================================
 * Power Management Notifier
 * ============================================================ */
/*
 * This is the CORRECT mechanism for a generic module.
 * Linux PM core calls this during system suspend/resume.
 */

static int pm_demo_notifier(struct notifier_block *nb,
                            unsigned long event, void *data)
{
    switch (event) {

    case PM_SUSPEND_PREPARE:
        switch (pm_level) {
        case 1:
            pr_info("pm_demo: entering IDLE-like suspend\n");
            break;
        case 2:
            pr_info("pm_demo: entering POWER-DOWN-like suspend\n");
            break;
        default:
            pr_info("pm_demo: entering NORMAL suspend\n");
            break;
        }
        break;

    case PM_POST_SUSPEND:
        pr_info("pm_demo: resume from suspend\n");
        break;
    }

    return NOTIFY_OK;
}

static struct notifier_block pm_demo_nb = {
    .notifier_call = pm_demo_notifier,
};

/* ============================================================
 * Module Init / Exit
 * ============================================================ */

static int __init pm_demo_init(void)
{
    int ret;

    /* Allocate device number */
    ret = alloc_chrdev_region(&dev_num, 0, 1, DEV_NAME);
    if (ret)
        return ret;

    /* Initialize cdev */
    cdev_init(&pm_cdev, &pm_fops);
    pm_cdev.owner = THIS_MODULE;

    ret = cdev_add(&pm_cdev, dev_num, 1);
    if (ret)
        goto err_cdev;

    /* Create class and device node */
    pm_class = class_create(DEV_NAME);
    if (IS_ERR(pm_class)) {
        ret = PTR_ERR(pm_class);
        goto err_class;
    }

    device_create(pm_class, NULL, dev_num, NULL, DEV_NAME);

    /* Register PM notifier */
    register_pm_notifier(&pm_demo_nb);

    pr_info("pm_demo loaded (pm_level=%d)\n", pm_level);
    return 0;

err_class:
    cdev_del(&pm_cdev);
err_cdev:
    unregister_chrdev_region(dev_num, 1);
    return ret;
}

static void __exit pm_demo_exit(void)
{
    unregister_pm_notifier(&pm_demo_nb);

    device_destroy(pm_class, dev_num);
    class_destroy(pm_class);
    cdev_del(&pm_cdev);
    unregister_chrdev_region(dev_num, 1);

    pr_info("pm_demo unloaded\n");
}

module_init(pm_demo_init);
module_exit(pm_demo_exit);

/* ============================================================
 * Module Metadata
 * ============================================================ */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Power management demo using module_param and PM notifier");
