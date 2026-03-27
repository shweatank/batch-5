#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>      // kmalloc, kfree
#include <linux/vmalloc.h>   // vmalloc, vfree
#include <linux/mm.h>        // si_meminfo()

#define BUF_SIZE 8192   // 8 KB buffer

static char *kmalloc_buf;
static char *vmalloc_buf;

static int __init memopt_init(void)
{
    struct sysinfo i;

    printk(KERN_INFO "=== Memory Optimization Project Started ===\n");

    // Allocate memory using kmalloc (physically contiguous)
    kmalloc_buf = kmalloc(BUF_SIZE, GFP_KERNEL);
    if (!kmalloc_buf) {
        printk(KERN_ERR "kmalloc failed\n");
        return -ENOMEM;
    }
    printk(KERN_INFO "kmalloc allocated %d bytes\n", BUF_SIZE);

    // Allocate memory using vmalloc (virtually contiguous)
    vmalloc_buf = vmalloc(BUF_SIZE);
    if (!vmalloc_buf) {
        printk(KERN_ERR "vmalloc failed\n");
        kfree(kmalloc_buf);
        return -ENOMEM;
    }
    printk(KERN_INFO "vmalloc allocated %d bytes\n", BUF_SIZE);

    // Show memory statistics
    si_meminfo(&i);
    printk(KERN_INFO "Total RAM: %lu KB\n", i.totalram << (PAGE_SHIFT - 10));
    printk(KERN_INFO "Free RAM: %lu KB\n", i.freeram << (PAGE_SHIFT - 10));
    printk(KERN_INFO "Shared RAM: %lu KB\n", i.sharedram << (PAGE_SHIFT - 10));
    printk(KERN_INFO "Buffer RAM: %lu KB\n", i.bufferram << (PAGE_SHIFT - 10));

    // ZRAM info (if enabled in system)
    printk(KERN_INFO "Check /sys/block/zram0/disksize for ZRAM size\n");
    printk(KERN_INFO "Check /sys/block/zram0/compr_data_size for compressed usage\n");

    printk(KERN_INFO "=== Memory Optimization Module Loaded ===\n");
    return 0;
}

static void __exit memopt_exit(void)
{
    if (kmalloc_buf) {
        kfree(kmalloc_buf);
        printk(KERN_INFO "kmalloc memory freed\n");
    }
    if (vmalloc_buf) {
        vfree(vmalloc_buf);
        printk(KERN_INFO "vmalloc memory freed\n");
    }

    printk(KERN_INFO "=== Memory Optimization Module Unloaded ===\n");
}

module_init(memopt_init);
module_exit(memopt_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kalisetti Likitha");
MODULE_DESCRIPTION("Linux Kernel 6.13 Memory Optimization Project with ZRAM Info");

