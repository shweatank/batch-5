#include "iaq_internal.h"

static int iaq_thread_fn(void *data)
{
    while (!kthread_should_stop()) {

        wait_event_interruptible(iaq->wq,
            atomic_read(&iaq->data_ready));

        atomic_set(&iaq->data_ready, 0);

        spin_lock(&iaq->buf_lock);
        struct iaq_sample latest =
            iaq->buffer[(iaq->head - 1 + IAQ_BUF_SIZE)
                        % IAQ_BUF_SIZE];
        spin_unlock(&iaq->buf_lock);

        pr_info("IAQ Value: %u\n", latest.value);

        if (latest.value > iaq->threshold)
            pr_warn("!!! ALERT TRIGGERED !!!\n");
    }

    return 0;
}

static int alert_thread_fn(void *data)
{
    struct msghdr msg = {};
    struct kvec iov;
    int ret;
    u32 alert_data = 1;

    while (!kthread_should_stop()) {

        wait_event_interruptible(iaq->alert_wq,
            atomic_read(&iaq->alert_flag));

        atomic_set(&iaq->alert_flag, 0);

        iov.iov_base = &alert_data;
        iov.iov_len  = sizeof(alert_data);

        ret = kernel_sendmsg(iaq->alert_sock,
                             &msg,
                             &iov,
                             1,
                             sizeof(alert_data));

        if (ret > 0)
            pr_info("Alert sent to PC\n");
        else
            pr_err("Alert send failed\n");
    }

    return 0;
}

void iaq_alert_thread_init(void)
{
    iaq->alert_thread =
        kthread_run(alert_thread_fn,
                    NULL,
                    "iaq_alert_thread");
}

void iaq_alert_thread_cleanup(void)
{
    if (iaq->alert_thread)
        kthread_stop(iaq->alert_thread);
}

void iaq_thread_init(void)
{
    iaq->thread =
        kthread_run(iaq_thread_fn,
                    NULL,
                    "iaq_thread");
}

void iaq_thread_cleanup(void)
{
    if (iaq->thread)
        kthread_stop(iaq->thread);
}

