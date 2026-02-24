#include "iaq_internal.h"

struct iaq_dev *iaq;

static int __init iaq_init(void)
{
    iaq = kzalloc(sizeof(*iaq), GFP_KERNEL);
    if (!iaq)
        return -ENOMEM;

    spin_lock_init(&iaq->buf_lock);
    mutex_init(&iaq->data_lock);
    init_waitqueue_head(&iaq->wq);
    init_waitqueue_head(&iaq->alert_wq);

    atomic_set(&iaq->data_ready, 0);
    atomic_set(&iaq->alert_flag, 0);

    iaq->head = 0;
    iaq->threshold = 1000;

    iaq_parse_dts();
    iaq_worker_init();
    iaq_thread_init();
    iaq_socket_init();
    iaq_alert_thread_init();
    iaq_timer_init();

    pr_info("IAQ driver loaded\n");
    return 0;
}

static void __exit iaq_exit(void)
{
    iaq_timer_cleanup();
    iaq_worker_cleanup();
    iaq_thread_cleanup();
    iaq_alert_thread_cleanup();
    iaq_socket_cleanup();

    kfree(iaq);

    pr_info("IAQ driver unloaded\n");
}

module_init(iaq_init);
module_exit(iaq_exit);

MODULE_LICENSE("GPL");

