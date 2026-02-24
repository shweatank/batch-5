#include "iaq_internal.h"

extern void iaq_buffer_push(struct iaq_sample *);

static void iaq_work_fn(struct work_struct *work)
{
    struct iaq_sample sample;

    mutex_lock(&iaq->data_lock);

    /* Hardcoded simulated data */
    sample.value = 750 + (jiffies % 500);
    sample.timestamp = ktime_get();

    mutex_unlock(&iaq->data_lock);

    iaq_buffer_push(&sample);

    atomic_set(&iaq->data_ready, 1);
    wake_up(&iaq->wq);

    if (sample.value > iaq->threshold) {
    	atomic_set(&iaq->alert_flag, 1);
    	wake_up(&iaq->alert_wq);
    }

}

static void iaq_tasklet_fn(unsigned long data)
{
    schedule_work(&iaq->work);
}

void iaq_worker_init(void)
{
    INIT_WORK(&iaq->work, iaq_work_fn);
    tasklet_init(&iaq->tasklet, iaq_tasklet_fn,
                 (unsigned long)iaq);
}

void iaq_worker_cleanup(void)
{
    tasklet_kill(&iaq->tasklet);
    cancel_work_sync(&iaq->work);
}

