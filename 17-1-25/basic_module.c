#include <linux/module.h>   // Required for all kernel modules
#include <linux/kernel.h>   // Required for printk()
#include <linux/init.h>     // Required for __init and __exit macros

/*
 * __init tells the kernel:
 * "This function is only needed during module loading."
 * After successful load, the memory used by this function
 * can be freed.
 */
static int __init basic_module_init(void)
{
    printk(KERN_INFO "Basic kernel module loaded\n");
    return 0;   // Returning 0 means successful load
}
static void __exit basic_module_exit(void)
{
    printk(KERN_INFO "Basic kernel module unloaded\n");
}

/*
 * These macros tell the kernel which functions
 * should be called when the module is inserted
 * and removed.
 */
module_init(basic_module_init);
module_exit(basic_module_exit);
MODULE_LICENSE("GPL");              // Prevents kernel taint
MODULE_AUTHOR("Prashant");
MODULE_DESCRIPTION("Most basic Linux kernel module for education");
