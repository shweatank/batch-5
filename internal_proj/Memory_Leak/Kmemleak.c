#include <linux/module.h>
#include <linux/slab.h>

static void *ptr;

static int __init kmemleak_init(void)
{
    ptr = kmalloc(128, GFP_KERNEL);
    pr_info("allocated and intentionally leaked\n");
    return 0;   // VERY IMPORTANT
}

static void __exit kmemleak_exit(void)
{
//	kfree(ptr);
    pr_info("exit without freeing memory\n");
}

module_init(kmemleak_init);
module_exit(kmemleak_exit);
MODULE_LICENSE("GPL");
