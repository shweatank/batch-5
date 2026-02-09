#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/interrupt.h>
#include <linux/smp.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("hrtimer schedules a tasklet (bottom-half demo)");
MODULE_VERSION("1.0");

static struct hrtimer td_timer;
static ktime_t td_period;
static unsigned long td_tick;

/*
 * Tasklet callback runs in softirq context:
 *  - atomic context (NO SLEEP)
 *  - cannot take mutexes
 *  - keep it short
 */
static void td_tasklet_fn(struct tasklet_struct *t)
{
    pr_info("td_tasklet: running cpu=%d tick=%lu\n",
            smp_processor_id(), td_tick);
}

/* Modern API: DECLARE_TASKLET(name, callback) */
DECLARE_TASKLET(td_tasklet, td_tasklet_fn);

/*
 * hrtimer callback is also atomic.
 * Here we schedule the tasklet as a deferred bottom-half.
 */
static enum hrtimer_restart td_hrtimer_cb(struct hrtimer *t)
{
    td_tick++;

    /* minimal top-half style work */
    pr_info("td_tasklet: timer fired cpu=%d tick=%lu (scheduling tasklet)\n",
            smp_processor_id(), td_tick);

    tasklet_schedule(&td_tasklet);

    hrtimer_forward_now(t, td_period);
    return HRTIMER_RESTART;
}

static int __init td_init(void)
{
    u64 period_ns = 500 * 1000 * 1000ULL; // 500ms

    pr_info("td_tasklet: init\n");

    td_period = ktime_set(0, period_ns);

    hrtimer_init(&td_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    td_timer.function = td_hrtimer_cb;

    td_tick = 0;
    hrtimer_start(&td_timer, td_period, HRTIMER_MODE_REL);

    pr_info("td_tasklet: started period_ns=%llu\n", period_ns);
    return 0;
}

static void __exit td_exit(void)
{
    int ret;

    pr_info("td_tasklet: exit\n");

    ret = hrtimer_cancel(&td_timer);
    pr_info("td_tasklet: hrtimer_cancel ret=%d\n", ret);

    /*
     * Ensure tasklet is not running and is killed before module unload.
     * tasklet_kill() waits for completion if running.
     */
    tasklet_kill(&td_tasklet);

    pr_info("td_tasklet: done\n");
}

module_init(td_init);
module_exit(td_exit);
