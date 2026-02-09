#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>     // kmalloc, kfree
#include <linux/vmalloc.h>  // vmalloc, vfree

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba Example");
MODULE_DESCRIPTION("Demo for kmalloc and vmalloc with GFP flags");

static size_t km_size = 4096;
module_param(km_size, ulong, 0644);
MODULE_PARM_DESC(km_size, "Size for kmalloc");

static size_t vm_size = 1024 * 1024; // 1 MB
module_param(vm_size, ulong, 0644);
MODULE_PARM_DESC(vm_size, "Size for vmalloc");

static void *km_ptr = NULL;
static void *vm_ptr = NULL;

static int __init mem_test_init(void)
{
    pr_info("mem_test: loading module\n");

    /* --------- kmalloc() example --------- */
    pr_info("mem_test: kmalloc %zu bytes\n", km_size);

    km_ptr = kmalloc(km_size, GFP_KERNEL);  // Important: GFP flags explained below
    if (!km_ptr) {
        pr_err("mem_test: kmalloc failed\n");
        return -ENOMEM;
    }

    strcpy(km_ptr, "Hello from kmalloc region!");
    pr_info("mem_test: kmalloc ptr=%px data='%s'\n", km_ptr, (char*)km_ptr);

    /* --------- vmalloc() example --------- */
    pr_info("mem_test: vmalloc %zu bytes\n", vm_size);

    vm_ptr = vmalloc(vm_size);
    if (!vm_ptr) {
        pr_err("mem_test: vmalloc failed\n");
        kfree(km_ptr);
        return -ENOMEM;
    }

    strcpy(vm_ptr, "Hello from vmalloc region!");
    pr_info("mem_test: vmalloc ptr=%px data='%s'\n", vm_ptr, (char*)vm_ptr);

    return 0;
}

static void __exit mem_test_exit(void)
{
    pr_info("mem_test: unloading module\n");

    if (km_ptr) {
        pr_info("mem_test: freeing kmalloc memory\n");
        kfree(km_ptr);
    }

    if (vm_ptr) {
        pr_info("mem_test: freeing vmalloc memory\n");
        vfree(vm_ptr);
    }

    pr_info("mem_test: done\n");
}

module_init(mem_test_init);
module_exit(mem_test_exit);
