#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>

static int __init helloworld_init(void){

printk(KERN_INFO "Hello world\n");
printk(KERN_INFO "My 1st driver\n");

return 0;
}

static void __exit helloworld_exit(void){

printk(KERN_INFO "Bye...\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JUGAL KISHORE");
MODULE_DESCRIPTION("A simple helloworld");
