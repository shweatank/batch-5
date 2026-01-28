#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>

static int *p=Null;

static int __init key_init(void)
{
	int num=10/0;
	while(1);

	printk(KERN_INFO "num=%d\n",num);
	return 0;
}

static void __exit key_exit(void)
{
	printk(KERN_INFO "Module unloaded\n");
}

module_init(key_init);
module_init(key_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ANVITHA KP");
MODULE_DESCRIPTION("hgfjhbhjkj");
