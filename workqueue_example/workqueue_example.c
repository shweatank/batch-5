#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>
#include <linux/delay.h>

// Forward declarations (important!)
static void work_handler(struct work_struct *work);
static void delayed_work_handler(struct work_struct *work);

static struct workqueue_struct *my_wq;

// Now DECLARE_WORK can see the function names
static DECLARE_WORK(my_work, work_handler);
static DECLARE_DELAYED_WORK(my_delayed_work, delayed_work_handler);

static void work_handler(struct work_struct *work)
{
    pr_info("Workqueue: Normal work executed\n");
}

static void delayed_work_handler(struct work_struct *work)
{
    pr_info("Workqueue: Delayed work executed after 2 seconds\n");
}

static int __init mymod_init(void)
{
    pr_info("Workqueue example loaded\n");

    my_wq = alloc_workqueue("my_wq", WQ_UNBOUND, 0);

    queue_work(my_wq, &my_work);

    queue_delayed_work(my_wq, &my_delayed_work, msecs_to_jiffies(2000));

    return 0;
}

static void __exit mymod_exit(void)
{
    pr_info("Workqueue example unloaded\n");

    cancel_delayed_work_sync(&my_delayed_work);
    flush_workqueue(my_wq);
    destroy_workqueue(my_wq);
}

module_init(mymod_init);
module_exit(mymod_exit);

MODULE_LICENSE("GPL");

