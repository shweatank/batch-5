#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
int simple_fun1_init(void)
{
	printk(KERN_INFO"hello from %s\n",__FUNCTION__);
	return 0;
}

void simple_fun1_exit(void)
{
	pr_info("hello from simple fun1 exit\n");
}

module_init(simple_fun1_init);
module_exit(simple_fun1_exit); //macros

MODULE_LICENSE("GPL");
MODULE_AUTHOR("prasanna");
MODULE_DESCRIPTION("simple module example");
