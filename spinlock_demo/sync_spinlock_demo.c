// SPDX-License-Identifier: GPL-2.0
/*
 * sync_spinlock_demo.c - Very simple spinlock example
 *
 * One timer increments a shared counter.
 * init() also increments once.
 * Both paths use a spinlock to protect 'shared'.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/spinlock.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("Simple spinlock synchronization demo");
MODULE_VERSION("1.0");

static int shared = 0;
static spinlock_t lock;
static struct timer_list my_timer;

/* Timer callback: runs in softirq context */
static void my_timer_func(struct timer_list *t)
{
	unsigned long flags;

	spin_lock_irqsave(&lock, flags);
	shared++;
	pr_info("spinlock_demo: timer increment, shared = %d\n", shared);
	spin_unlock_irqrestore(&lock, flags);

	/* Re-arm timer every 1 second */
	mod_timer(&my_timer, jiffies + HZ);
}

static int __init sync_spinlock_demo_init(void)
{
	unsigned long flags;

	pr_info("spinlock_demo: init\n");

	spin_lock_init(&lock);

	/* Protected increment in process context */
	spin_lock_irqsave(&lock, flags);
	shared++;
	pr_info("spinlock_demo: init increment, shared = %d\n", shared);
	spin_unlock_irqrestore(&lock, flags);

	/* Setup timer */
	timer_setup(&my_timer, my_timer_func, 0);
	mod_timer(&my_timer, jiffies + HZ);

	return 0;
}

static void __exit sync_spinlock_demo_exit(void)
{
	del_timer_sync(&my_timer);

	pr_info("spinlock_demo: exit, final shared = %d\n", shared);
}

module_init(sync_spinlock_demo_init);
module_exit(sync_spinlock_demo_exit);

