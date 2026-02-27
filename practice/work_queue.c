#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/workqueue.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("prasanna");
MODULE_DESCRIPTION("Simple Workqueue Example");

static struct workqueue_struct *my_wq;
static struct work_struct my_work;

static void work_handler(struct work_struct *work)
{
	pr_info("work queue handler..sleep allowed here\n");
	msleep(2000);
	pr_info("work completed\n");
}
static int __init work_init_fun(void)
{
	pr_info("init fun...creating work queue...initialising work...queue work\n");
	my_wq=create_singlethread_workqueue("my_work_queue");
	if(!my_wq)
		return -ENOMEM;
	INIT_WORK(&my_work,work_handler);
	queue_work(my_wq,&my_work);
	return 0;
}
static void __exit work_exit_fun(void)
{
	pr_info("exit fun\n");
	flush_workqueue(my_wq);
	destroy_workqueue(my_wq);
}
module_init(work_init_fun);
module_exit(work_exit_fun);
