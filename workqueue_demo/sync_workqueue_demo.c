// SPDX-License-Identifier: GPL-2.0
/*
 * sync_workqueue_demo.c - Very simple delayed_work example
 *
 * A delayed_work runs periodically, printing a message.
 * Module exit uses cancel_delayed_work_sync() to stop it safely.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/workqueue.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("Simple delayed_work + cancel_delayed_work_sync demo");
MODULE_VERSION("1.0");

static struct delayed_work my_work;

static void my_work_func(struct work_struct *w)
{
	static int count;

	count++;
	pr_info("workqueue_demo: work run #%d\n", count);

	/* Re-schedule again after 1 second */
	schedule_delayed_work(&my_work, HZ);
}

static int __init sync_workqueue_demo_init(void)
{
	pr_info("workqueue_demo: init\n");

	INIT_DELAYED_WORK(&my_work, my_work_func);

	/* First schedule after 1 second */
	schedule_delayed_work(&my_work, HZ);

	return 0;
}

static void __exit sync_workqueue_demo_exit(void)
{
	/* Ensure no more work is running or pending */
	cancel_delayed_work_sync(&my_work);

	pr_info("workqueue_demo: exit\n");
}

module_init(sync_workqueue_demo_init);
module_exit(sync_workqueue_demo_exit);
