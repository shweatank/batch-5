#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>

#define DEVICE_NAME "basic_char"

static int major_number;

/* Minimal file operations */
static struct file_operations fops = {
    .owner = THIS_MODULE,
};

/* Module initialization */
static int __init basic_char_init(void)
{
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        pr_err("Failed to register char device\n");
        return major_number;
    }

    pr_info("Basic char driver loaded\n");
    pr_info("Major number allocated: %d\n", major_number);
    return 0;
}

/* Module cleanup */
static void __exit basic_char_exit(void)
{
    unregister_chrdev(major_number, DEVICE_NAME);
    pr_info("Basic char driver unloaded\n");
}

module_init(basic_char_init);
module_exit(basic_char_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhi");
MODULE_DESCRIPTION("Minimal character device driver with init and exit only");

