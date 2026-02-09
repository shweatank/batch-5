#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/interrupt.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhi");
MODULE_DESCRIPTION("Timer + Workqueue + Waitqueue Linux Driver Example");

static struct timer_list my_timer;
static struct workqueue_struct *my_wq;
static DECLARE_WORK(my_work, NULL);
static wait_queue_head_t my_waitqueue;
static int condition = 0;

/* Workqueue function */
static void my_work_func(struct work_struct *work)
{
    pr_info("[Workqueue] Executing work function\n");
}

/* Timer callback */
static void my_timer_callback(struct timer_list *t)
{
    pr_info("[Timer] Timer fired, queueing work\n");

    /* Queue work in workqueue */
    if (my_wq)
        queue_work(my_wq, &my_work);

    /* Wake up wait queue */
    condition = 1;
    wake_up_interruptible(&my_waitqueue);

    /* Re-arm the timer */
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(2000));
}

/* Kernel thread that waits on wait queue */
static int waitqueue_thread(void *data)
{
    pr_info("[Waitqueue Thread] Started\n");
    while (!kthread_should_stop()) {
        wait_event_interruptible(my_waitqueue, condition != 0);
        pr_info("[Waitqueue Thread] Woken up by timer!\n");
        condition = 0;
        ssleep(1);
    }
    pr_info("[Waitqueue Thread] Exiting\n");
    return 0;
}

static struct task_struct *wait_thread;

/* Module init */
static int __init my_module_init(void)
{
    pr_info("Module loaded\n");

    /* Initialize wait queue */
    init_waitqueue_head(&my_waitqueue);

    /* Initialize workqueue */
    my_wq = create_singlethread_workqueue("my_workqueue");
    if (!my_wq) {
        pr_err("Failed to create workqueue\n");
        return -ENOMEM;
    }
    INIT_WORK(&my_work, my_work_func);

    /* Setup timer */
    timer_setup(&my_timer, my_timer_callback, 0);
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(2000));

    /* Start kernel thread waiting on waitqueue */
    wait_thread = kthread_run(waitqueue_thread, NULL, "waitqueue_thread");
    if (IS_ERR(wait_thread)) {
        pr_err("Failed to create waitqueue thread\n");
        destroy_workqueue(my_wq);
        del_timer(&my_timer);
        return PTR_ERR(wait_thread);
    }

    return 0;
}

/* Module exit */
static void __exit my_module_exit(void)
{
    pr_info("Module unloading\n");

    if (wait_thread)
        kthread_stop(wait_thread);

    del_timer_sync(&my_timer);

    if (my_wq) {
        flush_workqueue(my_wq);
        destroy_workqueue(my_wq);
    }

    pr_info("Module unloaded\n");
}

module_init(my_module_init);
module_exit(my_module_exit);
