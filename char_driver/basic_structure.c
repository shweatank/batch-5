/* basic structure for any kernel module*/

#include<linux/module.h>     //required for loadable modules
#include<linux/kernel.h>     //required for api like printk()
#include<linux/init.h>       //for macros init and exit

/*init function is executed when module is loaded*/
static int __init init_fun(void)
{
printk(KERN_INFO"module loaded\n");
return 0;
}
/*exit function is executed when module is unloaded*/
static void __exit exit_fun(void)
{
printk(KERN_INFO"module unloaded\n");
}

/*macros for init and exit*/
module_init(init_fun);
module_exit(exit_fun);


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("BASIC STRUCTURE OF MODULE");
MODULE_AUTHOR("VARSHITHA");
MODULE_VERSION("2:1.0");

