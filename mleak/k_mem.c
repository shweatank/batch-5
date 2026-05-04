#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/slab.h>

static char *p;

static int __init my_init(void)
{
printk(KERN_INFO "kernel memory allocation module loaded\n");

p=kmalloc(1024, GFP_KERNEL);

if(!p)
{
printk(KERN_ALERT "memory allocatiion failed\n");
return -ENOMEM;
}

printk(KERN_INFO "Memory allocated successfully at address: %p\n",p);

    strcpy(p, "Hello from Kernel Space!");
    printk(KERN_INFO "Stored Data: %s\n", p);

    return 0;
}

static void __exit my_exit(void)
{
    // Free allocated memory
    if (p) {
        kfree(p);
        printk(KERN_INFO "Memory freed\n");
    }

    printk(KERN_INFO "Module Unloaded\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sneha");
MODULE_DESCRIPTION("kernel space memory allocation");
