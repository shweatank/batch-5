#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/slab.h>

struct dev
{
	int val1;
	int val2;
};

static struct dev *ptr;

static int __init my_init(void)
{
ptr=kmalloc(sizeof(struct dev),GFP_KERNEL);

if(!ptr)
	return -ENOMEM;

		ptr->val1=10;
		ptr->val2=20;

		printk(KERN_INFO "device created:val1=%d, val2=%d\n",ptr->val1,ptr->val2);

		return 0;
}


static void __exit my_exit(void)
{
	kfree(ptr);
	printk(KERN_INFO "Device removed\n");

}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
