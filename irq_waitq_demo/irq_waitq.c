#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <linux/wait.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhi");
MODULE_DESCRIPTION("IRQ + Wait Queue example");

/* Keyboard IRQ for demo */
static int irq = 1;
module_param(irq, int, 0444);

/* Wait queue and condition */
static DECLARE_WAIT_QUEUE_HEAD(my_waitq);
static int irq_event = 0;

static struct task_struct *worker_thread;

/* ================= Worker Thread ================= */
static int wait_thread_fn(void *data)
{
    pr_info("wait_thread: started, waiting for IRQ\n");

    while (!kthread_should_stop()) {

        /* Sleep until IRQ signals event */
        wait_event_interruptible(my_waitq, irq_event == 1);

        /* Reset condition */
        irq_event = 0;

        pr_info("wait_thread: woke up, processing data\n");

        /* Heavy work allowed */
        msleep(1000);

        pr_info("wait_thread: work done\n");
    }

    return 0;
}

/* ================= IRQ Handler ================= */
static irqreturn_t irq_handler(int irq, void *dev_id)
{
    pr_info("irq_handler: IRQ %d received\n", irq);

    irq_event = 1;

    /* Wake sleeping thread */
    wake_up_interruptible(&my_waitq);

    return IRQ_HANDLED;
}

/* ================= Module Init ================= */
static int __init irq_waitq_init(void)
{
    int ret;

    pr_info("irq_waitq: init\n");

    worker_thread = kthread_run(wait_thread_fn, NULL, "irq_wait_thread");
    if (IS_ERR(worker_thread))
        return PTR_ERR(worker_thread);

    ret = request_irq(irq,
                      irq_handler,
                      IRQF_SHARED,
                      "irq_waitq_demo",
                      (void *)&irq);
    if (ret) {
        kthread_stop(worker_thread);
        return ret;
    }

    pr_info("irq_waitq: IRQ %d registered\n", irq);
    return 0;
}

/* ================= Module Exit ================= */
static void __exit irq_waitq_exit(void)
{
    pr_info("irq_waitq: exit\n");

    free_irq(irq, (void *)&irq);

    /* Wake thread so it can exit */
    irq_event = 1;
    wake_up_interruptible(&my_waitq);

    kthread_stop(worker_thread);
}

module_init(irq_waitq_init);
module_exit(irq_waitq_exit);
