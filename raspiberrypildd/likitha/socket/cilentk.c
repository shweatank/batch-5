#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Team2");
MODULE_DESCRIPTION("Safe kernel module with server IP and kthread");
MODULE_VERSION("1.0");

// Module parameter for server IP
static char *server_ip = "10.198.33.206";
module_param(server_ip, charp, 0444);
MODULE_PARM_DESC(server_ip, "Server IP address for the module");

// Pointer to kernel thread
static struct task_struct *kthread_task = NULL;

// Kernel thread function
static int kthread_fn(void *data)
{
    char *ip = (char *)data;

    printk(KERN_INFO "[kclient] Thread started, server IP: %s\n", ip);

    while (!kthread_should_stop()) {
        // Example: simulate some work
        msleep(1000);
        printk(KERN_INFO "[kclient] Heartbeat to %s\n", ip);
    }

    printk(KERN_INFO "[kclient] Thread stopping\n");
    return 0;
}

// Module initialization
static int __init kclient_init(void)
{
    printk(KERN_INFO "[kclient] Module loading, server IP: %s\n", server_ip);

    // Start the kernel thread
    kthread_task = kthread_run(kthread_fn, server_ip, "kclient_thread");
    if (IS_ERR(kthread_task)) {
        printk(KERN_ERR "[kclient] Failed to create kernel thread\n");
        kthread_task = NULL;
        return PTR_ERR(kthread_task);
    }

    printk(KERN_INFO "[kclient] Kernel thread started successfully\n");
    return 0;
}

// Module exit
static void __exit kclient_exit(void)
{
    printk(KERN_INFO "[kclient] Module exiting\n");

    // Stop the kernel thread safely
    if (kthread_task) {
        int ret = kthread_stop(kthread_task);
        printk(KERN_INFO "[kclient] Kernel thread stopped, return=%d\n", ret);
        kthread_task = NULL;
    } else {
        printk(KERN_WARNING "[kclient] Thread pointer is NULL, nothing to stop\n");
    }

    printk(KERN_INFO "[kclfconfigient] Module exited safely\n");
}

module_init(kclient_init);
module_exit(kclient_exit);

