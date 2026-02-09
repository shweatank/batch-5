#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/keyboard.h>
#include <linux/irq.h>
#include <linux/irqreturn.h>
#include <linux/init.h>
#include <linux/jiffies.h>


static struct tasklet_struct kb_tasklet;
static unsigned long key_count = 0;


void kb_tasklet_fn(unsigned long data)
{
    key_count++;
    pr_info("kb_tasklet: key pressed count=%lu\n", key_count);
}


static irqreturn_t kb_irq_handler(int irq, void *dev_id)
{
    
    tasklet_schedule(&kb_tasklet);
    return IRQ_HANDLED;
}

static int irq_number = 1; 

static int __init kb_tasklet_init(void)
{
    int ret;

    pr_info("kb_tasklet: module init\n");

 
    tasklet_init(&kb_tasklet, kb_tasklet_fn, 0);

    
    ret = request_irq(irq_number, kb_irq_handler, IRQF_SHARED, "kb_tasklet_irq", (void *)(kb_irq_handler));
    if (ret) {
        pr_err("kb_tasklet: failed to request IRQ %d\n", irq_number);
        tasklet_kill(&kb_tasklet);
        return ret;
    }

    pr_info("kb_tasklet: IRQ %d registered\n", irq_number);
    return 0;
}

static void __exit kb_tasklet_exit(void)
{
    free_irq(irq_number, (void *)(kb_irq_handler));
    tasklet_kill(&kb_tasklet);
    pr_info("kb_tasklet: module exit\n");
}

module_init(kb_tasklet_init);
module_exit(kb_tasklet_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhi");
MODULE_DESCRIPTION("Keyboard interrupt with tasklet demo");
MODULE_VERSION("1.0");
 

