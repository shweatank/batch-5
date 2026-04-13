#include<linux/module.h>
#include<linux/init.h>

static int __init panic_init(void)
{
	int *p=NULL;
	pr_info("panic is triggerd\n");
	*p=40;
	return 0;
}

static void __exit panic_exit(void)
{
	pr_info("exiting\n");
	return ;
}

module_init(panic_init);
module_exit(panic_exit);


MODULE_LICENSE("gpl");
