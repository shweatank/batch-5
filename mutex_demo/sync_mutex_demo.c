// SPDX-License-Identifier: GPL-2.0
/*
 * sync_mutex_demo.c - Very simple mutex example
 *
 * Two kernel threads increment a shared counter,
 * protected by a mutex. They sleep between updates.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/mutex.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("Simple mutex synchronization demo");
MODULE_VERSION("1.0");

static int shared = 0;
static struct mutex mtx;

static struct task_struct *thread1;
static struct task_struct *thread2;

static int worker_fn(void *arg)
{
	const char *name = (const char *)arg;

	while (!kthread_should_stop()) {
		mutex_lock(&mtx);
        shared++;
        pr_info("mutex_demo: %s increment, shared = %d\n", name, shared);
        mutex_unlock(&mtx);

        msleep(500);
	}

	return 0;
}

static int __init sync_mutex_demo_init(void)
{
	mutex_init(&mtx);

	thread1 = kthread_run(worker_fn, "thread1", "mutex_demo1");
	if (IS_ERR(thread1)) {
		pr_err("mutex_demo: failed to create thread1\n");
		return PTR_ERR(thread1);
	}

	thread2 = kthread_run(worker_fn, "thread2", "mutex_demo2");
	if (IS_ERR(thread2)) {
		pr_err("mutex_demo: failed to create thread2\n");
		kthread_stop(thread1);
		return PTR_ERR(thread2);
	}

	pr_info("mutex_demo: init done\n");
	return 0;
}

static void __exit sync_mutex_demo_exit(void)
{
	if (thread1)
		kthread_stop(thread1);
	if (thread2)
		kthread_stop(thread2);

	pr_info("mutex_demo: exit, final shared = %d\n", shared);
}

module_init(sync_mutex_demo_init);
module_exit(sync_mutex_demo_exit);

