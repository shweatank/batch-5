#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/workqueue.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Simple Workqueue Example");

static struct workqueue_struct *my_wq;
static struct work_struct my_work;

/* Workqueue handler */
static void work_handler(struct work_struct *work)
{
    pr_info("work_handler: started in process context\n");

    /* Allowed to sleep */
    msleep(2000);

    pr_info("work_handler: finished work\n");
}

static int __init workq_init(void)
{
    pr_info("workq_demo: init\n");

    /* Create dedicated workqueue */
    my_wq = create_singlethread_workqueue("techdhaba_wq");
    if (!my_wq)
        return -ENOMEM;

    INIT_WORK(&my_work, work_handler);

    /* Queue the work */
    queue_work(my_wq, &my_work);

    return 0;
}

static void __exit workq_exit(void)
{
    pr_info("workq_demo: exit\n");

    flush_workqueue(my_wq);
    destroy_workqueue(my_wq);
}

module_init(workq_init);
module_exit(workq_exit);
