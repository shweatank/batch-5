#include <linux/module.h>   // Needed for all modules
#include <linux/init.h>     // Needed for __init/__exit macros
#include <linux/kthread.h>  // Needed for kthread APIs
#include <linux/delay.h>    // Needed for ssleep()
static struct task_struct *task;

int thread_fn(void *data)
{
    while (!kthread_should_stop()) {
        pr_info("Kernel thread running...\n");
        ssleep(1);
    }
    pr_info("Kernel thread stopping...\n");
    return 0;
}

static int __init my_module_init(void)
{
    pr_info("Module init: starting kernel thread\n");
    task = kthread_run(thread_fn, NULL, "irq_worker");
    if (IS_ERR(task)) {
        pr_err("Failed to create kernel thread\n");
        return PTR_ERR(task);
    }
    return 0;
}

static void __exit my_module_exit(void)
{
    pr_info("Module exit: stopping kernel thread\n");
    if (task)
        kthread_stop(task);
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple kernel thread example");

