/*
 * basic_module.c
 *
 * This is the simplest possible Linux kernel module.
 * It does NOT control any hardware.
 * It only demonstrates how code is loaded into
 * and removed from the Linux kernel.
 */

#include <linux/module.h>   // Required for all kernel modules
#include <linux/kernel.h>   // Required for printk()
#include <linux/init.h>     // Required for __init and __exit macros
static int __init basic_module_init(void)
{
    printk(KERN_INFO "Basic kernel module loaded\n");
    return 0;   // Returning 0 means successful load
}
static void __exit basic_module_exit(void)
{
    printk(KERN_INFO "Basic kernel module unloaded\n");
}

module_init(basic_module_init);
module_exit(basic_module_exit);


MODULE_LICENSE("GPL");              // Prevents kernel taint
MODULE_AUTHOR("Prashant");
MODULE_DESCRIPTION("Most basic Linux kernel module for education");
