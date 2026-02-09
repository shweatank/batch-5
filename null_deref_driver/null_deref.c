#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init null_deref_init(void)
{
    int *p = NULL;

    printk(KERN_INFO "About to dereference NULL pointer\n");

    *p = 42;  

    return 0;
}

static void __exit null_deref_exit(void)
{
    printk(KERN_INFO "Module unloaded\n");
}

module_init(null_deref_init);
module_exit(null_deref_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Test");
MODULE_DESCRIPTION("NULL pointer dereference test module");
