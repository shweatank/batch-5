#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>

#define KBD_IRQ        1
#define KBD_DATA_PORT  0x60
#define PROC_NAME      "irq_proc"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Demo");
MODULE_DESCRIPTION("IRQ + Workqueue + proc");

/* -------- Data structure -------- */
struct d {
    int n1;
    int n2;
    int result;
};

static struct d data;

/* -------- Workqueue -------- */
static struct workqueue_struct *my_wq;
static struct work_struct irq_work;

/* -------- Keymap (limited) -------- */
static const char *keymap[128] = {
    [0x1E] = "A",   /* Add */
    [0x1F] = "S",   /* Sub */
    [0x20] = "D",   /* Div */
    [0x32] = "M",   /* Mul */
};

/* -------- Workqueue handler (bottom half) -------- */
static void work_handler(struct work_struct *work)
{
    unsigned char scancode;
    const char *key;

    scancode = inb(KBD_DATA_PORT) & 0x7F;
    key = keymap[scancode];

    if (!key)
        return;

    switch (key[0]) {
    case 'A':
        data.result = data.n1 + data.n2;
        break;
    case 'S':
        data.result = data.n1 - data.n2;
        break;
    case 'M':
        data.result = data.n1 * data.n2;
        break;
    case 'D':
        if (data.n2 != 0)
            data.result = data.n1 / data.n2;
        break;
    }

    pr_info("Result = %d\n", data.result);
}

/* -------- IRQ handler (top half) -------- */
static irqreturn_t irq_handler(int irq, void *dev_id)
{
    queue_work(my_wq, &irq_work);
    return IRQ_HANDLED;
}

/* -------- /proc write -------- */
static ssize_t proc_write(struct file *file,
                          const char __user *buf,
                          size_t count,
                          loff_t *ppos)
{
    if (copy_from_user(&data, buf, sizeof(data)))
        return -EFAULT;

    return count;
}

/* -------- /proc read -------- */
static ssize_t proc_read(struct file *file,
                         char __user *buf,
                         size_t count,
                         loff_t *ppos)
{
    if (*ppos > 0)
        return 0;

    if (copy_to_user(buf, &data, sizeof(data)))
        return -EFAULT;

    *ppos = sizeof(data);
    return sizeof(data);
}

static const struct proc_ops proc_fops = {
    .proc_read  = proc_read,
    .proc_write = proc_write,
};

/* -------- Module init -------- */
static int __init irq_wq_init(void)
{
    int ret;

    my_wq = create_singlethread_workqueue("irq_wq");
    if (!my_wq)
        return -ENOMEM;

    INIT_WORK(&irq_work, work_handler);

    ret = request_irq(KBD_IRQ, irq_handler,
                      IRQF_SHARED,
                      "irq_wq_demo",
                      &data);
    if (ret) {
        destroy_workqueue(my_wq);
        return ret;
    }

    proc_create(PROC_NAME, 0666, NULL, &proc_fops);

    pr_info("IRQ + PROC module loaded\n");
    return 0;
}

/* -------- Module exit -------- */
static void __exit irq_wq_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    free_irq(KBD_IRQ, &data);
    flush_workqueue(my_wq);
    destroy_workqueue(my_wq);

    pr_info("IRQ + PROC module unloaded\n");
}

module_init(irq_wq_init);
module_exit(irq_wq_exit);

