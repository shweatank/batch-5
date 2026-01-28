#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>

static int a=10;
static int b=a/0;

static int FPE_init(void)
{
	printk(KERN_INFO "result=%d\n",b);
	printk("Module loaded successfully\n");
	return 0;
}

static void FPE_exit(void)
{
	printk("Module unloaded successfully\n");
}

module_init(FPE_init);
module_exit(FPE_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ANVITHA KP");
MODULE_DESCRIPTION("Floating point exception");
