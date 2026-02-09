#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Kernel exception example - divide by zero");

static int __init exception_init(void)
{
    int a = 1;
    int b = 0;
    int c;

    printk(KERN_INFO "TechDhaba: Loading exception module\n");
    printk(KERN_INFO "TechDhaba: Triggering divide-by-zero fault...\n");

    c = a / b;   // <-- This creates #DE exception
    printk(KERN_INFO "Result = %d\n", c);

    return 0;
}

static void __exit exception_exit(void)
{
    printk(KERN_INFO "TechDhaba: Exception module unloaded\n");
}

module_init(exception_init);
module_exit(exception_exit);
