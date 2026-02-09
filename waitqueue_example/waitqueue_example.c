#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/wait.h>
#include <linux/delay.h>
#include <linux/timer.h>

static struct task_struct *consumer_thread;
static wait_queue_head_t my_wq;
static int condition = 0;

// A timer acting as producer
static struct timer_list my_timer;

static void timer_callback(struct timer_list *t)
{
    pr_info("Timer fired: waking kthread\n");
    condition = 1;
    wake_up(&my_wq);   // wake up the waiting thread

    // Fire timer again in 3 seconds
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(3000));
}

static int consumer_fn(void *data)
{
    while (!kthread_should_stop()) {
        pr_info("kthread: waiting for condition...\n");

        wait_event(my_wq, condition == 1);

        pr_info("kthread: condition met! Processing work...\n");
        condition = 0;

        ssleep(1);  // simulate task
    }
    return 0;
}

static int __init mymod_init(void)
{
    pr_info("Waitqueue example loaded\n");

    init_waitqueue_head(&my_wq);

    // Start consumer kthread
    consumer_thread = kthread_run(consumer_fn, NULL, "consumer_thread");

    // Create and start timer (producer)
    timer_setup(&my_timer, timer_callback, 0);
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(3000));

    return 0;
}

static void __exit mymod_exit(void)
{
    pr_info("Waitqueue example removed\n");

    kthread_stop(consumer_thread);
    del_timer_sync(&my_timer);
}

module_init(mymod_init);
module_exit(mymod_exit);

MODULE_LICENSE("GPL");
