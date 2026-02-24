#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>

#define DRIVER_NAME "hrtimer_only_driver"
#define TIMER_INTERVAL_MS 1000  // 1 second

/* Device structure holding the timer */
struct my_device {
    struct hrtimer timer;
    ktime_t interval;
};

static struct my_device dev;

/* Timer callback function */
static enum hrtimer_restart my_timer_callback(struct hrtimer *timer)
{
    pr_info("hrtimer fired!\n");

    /* Move timer forward to maintain periodic firing */
    hrtimer_forward_now(timer, dev.interval);

    /* Return restart to keep timer periodic */
    return HRTIMER_RESTART;
}

/* Module initialization */
static int __init my_driver_init(void)
{
    pr_info("%s: Loading driver\n", DRIVER_NAME);

    /* Set timer interval (convert ms to ktime) */
    dev.interval = ms_to_ktime(TIMER_INTERVAL_MS);

    /* Initialize high-resolution timer */
    hrtimer_init(&dev.timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);

    /* Assign callback */
    dev.timer.function = my_timer_callback;

    /* Start timer */
    hrtimer_start(&dev.timer, dev.interval, HRTIMER_MODE_REL);

    return 0;
}

/* Module cleanup */
static void __exit my_driver_exit(void)
{
    int ret;

    /* Cancel the timer */
    ret = hrtimer_cancel(&dev.timer);
    if (ret)
        pr_info("%s: Timer was active when cancelled\n", DRIVER_NAME);

    pr_info("%s: Unloading driver\n", DRIVER_NAME);
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Minimal hrtimer-only kernel module");

