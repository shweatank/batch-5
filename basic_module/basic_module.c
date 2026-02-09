#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init basic_module_init(void)
{
    printk(KERN_INFO "Basic kernel module loaded\n");
    return 0;
}

static void __exit basic_module_exit(void)
{
    printk(KERN_INFO "Basic kernel module unloaded\n");
}

module_init(basic_module_init);
module_exit(basic_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prashant");
MODULE_DESCRIPTION("Most basic Linux kernel module for education");
