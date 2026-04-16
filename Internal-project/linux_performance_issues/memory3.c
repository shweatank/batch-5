#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/sysinfo.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
static void *kmalloc_buf;
static void *vmalloc_buf;
void print_meminfo(void)
{
    struct sysinfo si;
    si_meminfo(&si);
    printk(KERN_INFO "Total RAM  : %lu MB\n",
           (si.totalram * si.mem_unit) / (1024 * 1024));

    printk(KERN_INFO "Free RAM   : %lu MB\n",
           (si.freeram * si.mem_unit) / (1024 * 1024));

    printk(KERN_INFO "Shared RAM : %lu MB\n",
           (si.sharedram * si.mem_unit) / (1024 * 1024));

    printk(KERN_INFO "Buffer RAM : %lu MB\n",
           (si.bufferram * si.mem_unit) / (1024 * 1024));
}
static int __init mem_project_init(void)
{
    printk(KERN_INFO "Memory Optimization Module Loaded\n");
     printk(KERN_INFO "Before Allocation:\n");
    print_meminfo();
    kmalloc_buf = kmalloc(1024 * 1024, GFP_KERNEL);   
    vmalloc_buf = vmalloc(1024 * 1024);              
     if (!kmalloc_buf || !vmalloc_buf) {
        printk(KERN_ERR "Memory allocation failed\n");
       if (kmalloc_buf)
            kfree(kmalloc_buf);
       return -ENOMEM;
    }
    printk(KERN_INFO "Allocated 1MB using kmalloc + vmalloc\n");
    printk(KERN_INFO "After Allocation:\n");
    print_meminfo();
    return 0;
}
static void __exit mem_project_exit(void)
{
    printk(KERN_INFO "Memory Optimization Module Unloaded\n");

    if (kmalloc_buf) {
        kfree(kmalloc_buf);
        printk(KERN_INFO "Freed kmalloc memory\n");
    }
   if (vmalloc_buf) {
        vfree(vmalloc_buf);
        printk(KERN_INFO "Freed vmalloc memory\n");
    }
    printk(KERN_INFO "After Freeing Memory:\n");
    print_meminfo();
}
module_init(mem_project_init);
module_exit(mem_project_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Likitha");
MODULE_DESCRIPTION("Memory Optimization Project - Linux Kernel 6.13");
