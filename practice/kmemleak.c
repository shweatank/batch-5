#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/slab.h>
static int __init kmemleak_init(void)
{
	char *p;
	pr_info("kmemleak init function\n");
	p=kmalloc(1024,GFP_KERNEL);
	//leaked
	return 0;
}
static void __exit kmemleak_exit(void)
{
	pr_info("kmemleak exit function\n");
}

module_init(kmemleak_init);
module_exit(kmemleak_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prasanna");
MODULE_DESCRIPTION("kmemleak basic understanding example");
