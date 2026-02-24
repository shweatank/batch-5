#include "iaq_internal.h"

static enum hrtimer_restart iaq_timer_cb(struct hrtimer *t)
{
    tasklet_schedule(&iaq->tasklet);
    hrtimer_forward_now(t, iaq->interval);
    return HRTIMER_RESTART;
}

void iaq_timer_init(void)
{
    iaq->interval = ms_to_ktime(5000);

    hrtimer_init(&iaq->timer,
                 CLOCK_MONOTONIC,
                 HRTIMER_MODE_REL);

    iaq->timer.function = iaq_timer_cb;

    hrtimer_start(&iaq->timer,
                  iaq->interval,
                  HRTIMER_MODE_REL);
}

void iaq_timer_cleanup(void)
{
    hrtimer_cancel(&iaq->timer);
}

