#include <linux/kernel.h>    // pr_info
#include <linux/module.h>    // module macros
#include <linux/fs.h>        // file operations
#include <linux/init.h>      // __init/__exit
#include <linux/kthread.h>   // kthreads
#include <linux/wait.h>      // wait queues
#include <linux/err.h>       // IS_ERR

static int major = 0;
#define DEVICE_NAME "basic_wait"

DECLARE_WAIT_QUEUE_HEAD(wq);
static struct task_struct *wait_thread;
static int flag = 0;

// Thread function
static int wait_function(void *p)
{
    while (!kthread_should_stop())
    {
        pr_info("Thread: waiting for an event...\n");

        // Wait until flag != 0 or interrupted
        int ret = wait_event_interruptible(wq, flag != 0);
        if (ret == -ERESTARTSYS) {
            pr_info("Thread: wait interrupted\n");
            continue;
        }

        pr_info("Thread: event completed, flag=%d\n", flag);
        flag = 0;  // reset flag
    }

    pr_info("Thread: stopping\n");
    return 0;
}

// File operations
static int my_open(struct inode *inode, struct file *file)
{
    pr_info("Driver: open called\n");
    return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
    pr_info("Driver: close called\n");
    return 0;
}

static ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *offset)
{
    pr_info("Driver: write called\n");
    return len;
}

static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
    pr_info("Driver: read called\n");
    flag = 1;                       // set flag
    wake_up_interruptible(&wq);     // wake up waitqueue
    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write,
};

// Module init
static int __init wait_init(void)
{
    // Register char device
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        pr_err("Driver: failed to register major\n");
        return major;
    }
    pr_info("Driver loaded, major=%d\n", major);

    // Create kernel thread
    wait_thread = kthread_create(wait_function, NULL, "wait_thread");
    if (IS_ERR(wait_thread)) {
        pr_err("Driver: thread creation failed\n");
        unregister_chrdev(major, DEVICE_NAME);
        return PTR_ERR(wait_thread);
    }

    pr_info("Driver: thread created successfully\n");
    wake_up_process(wait_thread);

    return 0;
}

// Module exit
static void __exit wait_exit(void)
{
    if (wait_thread)
        kthread_stop(wait_thread);     // stop thread

    unregister_chrdev(major, DEVICE_NAME);
    wake_up_interruptible(&wq);

    pr_info("Driver removed\n");
}

module_init(wait_init);
module_exit(wait_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX <embetronicx@gmail.com>");
MODULE_DESCRIPTION("Simple Linux driver using waitqueue (fixed version)");
MODULE_VERSION("1.9");

