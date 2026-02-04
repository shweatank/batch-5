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

static int n1=10,n2=30,flag;

#include <linux/io.h>

#define KBD_IRQ        1
#define KBD_DATA_PORT  0x60

/* Simple US keyboard scan code map (Set 1) */
static const char *keymap[128] = {
    [0x01] = "ESC",
    [0x02] = "1",   [0x03] = "2",   [0x04] = "3",
    [0x05] = "4",   [0x06] = "5",   [0x07] = "6",
    [0x08] = "7",   [0x09] = "8",   [0x0A] = "9",
    [0x0B] = "0",
    [0x0C] = "-",   [0x0D] = "=",
    [0x0E] = "BACKSPACE",
    [0x0F] = "TAB",
    [0x10] = "Q",   [0x11] = "W",   [0x12] = "E",
    [0x13] = "R",   [0x14] = "T",   [0x15] = "Y",
    [0x16] = "U",   [0x17] = "I",   [0x18] = "O",
    [0x19] = "P",
    [0x1C] = "ENTER",
    [0x1D] = "CTRL",
    [0x1E] = "A",   [0x1F] = "S",   [0x20] = "D",
    [0x21] = "F",   [0x22] = "G",   [0x23] = "H",
    [0x24] = "J",   [0x25] = "K",   [0x26] = "L",
    [0x2C] = "Z",   [0x2D] = "X",   [0x2E] = "C",
    [0x2F] = "V",   [0x30] = "B",   [0x31] = "N",
    [0x32] = "M",
    [0x39] = "SPACE"
};


static irqreturn_t keyboard_isr(int irq, void *dev_id)
{
    unsigned char scancode;
    bool released;
    const char *key;

    scancode = inb(KBD_DATA_PORT);

    released = scancode & 0x80;
    scancode &= 0x7F;

    key = keymap[scancode];

    if (!key)
        key = "UNKNOWN";

    if (released)
    {
	    if(key[0]=='A')
	    {
		    printk("sum : %d\n",n1+n2);
	    }
        printk(KERN_INFO "kbd_irq: Key RELEASED -> %s (scancode 0x%02x)\n",
               key, scancode);
    }
    else
        printk(KERN_INFO "kbd_irq: Key PRESSED  -> %s (scancode 0x%02x)\n",
               key, scancode);

    return IRQ_HANDLED;
}

irqreturn_t(*key_isr)(int,void*)=keyboard_isr;

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
DECLARE_TASKLET(td_tasklet, keyboard_isr);

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
    
    tasklet_schedule(&);

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
