#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>

static int * ptr=0;

static int  __init my_init(void)
{
static int num=10;
	while(1)
		num=num/0;
printk(KERN_INFO "Dereferencing null ptr:%d\n",*ptr);
return 0;


}

static void __exit my_exit(void)
{
printk(KERN_INFO "__exit"); 
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
