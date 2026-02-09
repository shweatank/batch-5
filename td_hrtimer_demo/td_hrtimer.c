// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/smp.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhi");
MODULE_DESCRIPTION("Periodic hrtimer demo (timer-interrupt style callback)");
MODULE_VERSION("1.0");

static struct hrtimer td_timer;
static ktime_t td_period;
static unsigned long td_count;

/*
 * hrtimer callback runs in atomic context (do NOT sleep here).
 * Treat it like an interrupt-like context:
 *  - No blocking calls
 *  - No mutex_lock()
 *  - No msleep()
 */
static enum hrtimer_restart td_hrtimer_cb(struct hrtimer *t)
{
    td_count++;

    pr_info("td_hrtimer: tick=%lu cpu=%d\n", td_count, smp_processor_id());

    /*
     * Forward the timer to fire again (periodic behavior).
     * This keeps cadence stable.
     */
    hrtimer_forward_now(t, td_period);

    return HRTIMER_RESTART;
}

static int __init td_init(void)
{
    u64 period_ns = 1000000000ULL; // 1 second default

    pr_info("td_hrtimer: init\n");

    td_period = ktime_set(0, period_ns);

    hrtimer_init(&td_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    td_timer.function = td_hrtimer_cb;

    td_count = 0;
    hrtimer_start(&td_timer, td_period, HRTIMER_MODE_REL);

    pr_info("td_hrtimer: started period_ns=%llu\n", period_ns);
    return 0;
}

static void __exit td_exit(void)
{
    int ret;

    pr_info("td_hrtimer: exit\n");

    ret = hrtimer_cancel(&td_timer);
    pr_info("td_hrtimer: cancelled ret=%d (1 means callback was running)\n", ret);
}

module_init(td_init);
module_exit(td_exit);
