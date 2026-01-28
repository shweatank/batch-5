#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <linux/wait.h>
#include <linux/io.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("techdhaba");
MODULE_DESCRIPTION("Simple IRQ + kthread example");

/* Keyboard */
#define KBD_IRQ   1
#define KBD_PORT  0x60

static struct task_struct *worker_thread;
static DECLARE_WAIT_QUEUE_HEAD(wq);

static int irq_event = 0;
static unsigned char last_scancode;

/* ---------------- IRQ HANDLER ---------------- */
static irqreturn_t keyboard_isr(int irq, void *dev_id)
{
    /* Read scan code */
    last_scancode = inb(KBD_PORT);

    /* Signal thread */
    irq_event = 1;
    wake_up_interruptible(&wq);

    return IRQ_HANDLED;
}

/* ---------------- KTHREAD ---------------- */
static int irq_worker_fn(void *data)
{
    pr_info("irq_kthread: worker thread started\n");

    while (!kthread_should_stop()) {

        /* Sleep until IRQ wakes us */
        wait_event_interruptible(wq,
                                 irq_event || kthread_should_stop());

        if (kthread_should_stop())
            break;

        /* Process event */
        pr_info("irq_kthread: scancode received = 0x%02x\n",
                last_scancode);

        irq_event = 0;
    }

    pr_info("irq_kthread: worker thread stopping\n");
    return 0;
}

/* ---------------- MODULE INIT ---------------- */
static int __init irq_kthread_init(void)
{
    int ret;

    pr_info("irq_kthread: module loading\n");

    worker_thread = kthread_run(irq_worker_fn, NULL, "irq_worker");
    if (IS_ERR(worker_thread)) {
        pr_err("irq_kthread: failed to create thread\n");
        return PTR_ERR(worker_thread);
    }

    ret = request_irq(KBD_IRQ,
                      keyboard_isr,
                      IRQF_SHARED,
                      "irq_kthread_kbd",
                      (void *)keyboard_isr);

    if (ret) {
        pr_err("irq_kthread: request_irq failed\n");
        kthread_stop(worker_thread);
        return ret;
    }

    pr_info("irq_kthread: module loaded successfully\n");
    return 0;
}

/* ---------------- MODULE EXIT ---------------- */
static void __exit irq_kthread_exit(void)
{
    pr_info("irq_kthread: module unloading\n");

    free_irq(KBD_IRQ, (void *)keyboard_isr);

    if (worker_thread)
        kthread_stop(worker_thread);

    wake_up_interruptible(&wq);
}

module_init(irq_kthread_init);
module_exit(irq_kthread_exit);
