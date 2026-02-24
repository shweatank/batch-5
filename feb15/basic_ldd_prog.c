#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>

static int __init basic_init(void)
{
	printk(KERN_INFO"kernel module initialised\n");
	return 0;
}

static void __exit basic_exit(void)
{
	printk("kernel module unloaded\n");
}

module_init(basic_init);
module_exit(basic_exit);

MODULE_AUTHOR("praneeth");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LDD BASIC DRIVER");
