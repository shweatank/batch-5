#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

int export_fun(void)
{
	pr_info("created a fun:%s to understand exporting\n",__FUNCTION__);
	return 0;
}

EXPORT_SYMBOL(export_fun);
static int __init simple_fun1_init(void)
{
	printk(KERN_INFO"hello from %s\n",__FUNCTION__);
	return 0;
}

static void __exit simple_fun1_exit(void)
{
	pr_info("hello from %s\n",__FUNCTION__);
}

module_init(simple_fun1_init);
module_exit(simple_fun1_exit); //macros

MODULE_LICENSE("GPL");
MODULE_AUTHOR("prasanna");
MODULE_DESCRIPTION("simple module example");
