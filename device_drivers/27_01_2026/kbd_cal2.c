#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/miscdevice.h>

#define KBD_IRQ        1
#define KBD_DATA_PORT  0x60

/* Simple US keyboard scan code map (Set 1) */
unsigned char scancode;
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
struct cal
{
        int a;
        int b;
        char ch;
        long res;
};
struct cal c;
//static struct hrtimer td_timer;
//static ktime_t td_period;
//static unsigned long td_tick;

/*
 * Tasklet callback runs in softirq context:
 *  - atomic context (NO SLEEP)
 *  - cannot take mutexes
 *  - keep it short
 */
static void td_tasklet_fn(struct tasklet_struct *t)
{    bool released;
    const char *key;
    scancode = inb(KBD_DATA_PORT);

    released = scancode & 0x80;
    scancode &= 0x7F;

    key = keymap[scancode];

    if (!key)
        key = "UNKNOWN";

    if (released)
        printk(KERN_INFO "kbd_irq: Key RELEASED -> %s (scancode 0x%02x)\n",
               key, scancode);
   // else
     //   printk(KERN_INFO "kbd_irq: Key PRESSED  -> %s (scancode 0x%02x)\n",
       //        key, scancode);
    if(scancode==0x1e)
    {
            printk("RESULT of %d and %d with Addition is: %d \n",c.a,c.b,c.a+c.b);
    }
    else if(scancode==0x1f)
    {
            printk("RESULT of %d and %d with SUbstraction is: %d \n",c.a,c.b,c.a-c.b);
    }
    else if(scancode==0x32)
    {
            printk("RESULT of %d and %d with Multiplication is: %d \n",c.a,c.b,c.a*c.b);
    }
  //  else if(scancode==0x20)
   // {
     //       printk("RESULT of %d and %d with Division is: %d \n",c.a,c.b,c.a/c.b);
    //}


   // pr_info("td_tasklet: running cpu=%d tick=%lu\n",
      //      smp_processor_id(), td_tick);
}

/* Modern API: DECLARE_TASKLET(name, callback) */
DECLARE_TASKLET(td_tasklet, td_tasklet_fn);


static ssize_t kbd_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    if (count != sizeof(c))
        return -EINVAL;

    if (copy_from_user(&c, buf, sizeof(c)))
        return -EFAULT;

    printk(KERN_INFO "User data received: a=%d, b=%d\n", c.a, c.b);
    return count;
}
static const struct file_operations kbd_fops = {
    .owner = THIS_MODULE,
    .write = kbd_write,
};
static struct miscdevice kbd_miscdev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "kbd_calc4",
    .fops = &kbd_fops,
};

static irqreturn_t keyboard_isr(int irq, void *dev_id)
{
	bool released;
    const char *key;
    scancode = inb(KBD_DATA_PORT);

    released = scancode & 0x80;
    scancode &= 0x7F;

    key = keymap[scancode];

    if (!key){
        key = "UNKNOWN";
    }
    else{
	tasklet_schedule(&td_tasklet);
	}    
	return IRQ_HANDLED;
}


static int __init kbd_init(void)
{
    int ret;

    printk(KERN_INFO "kbd_irq: Initializing keyboard IRQ driver\n");

    ret = request_irq(KBD_IRQ,
                      keyboard_isr,
                      IRQF_SHARED,
                      "kbd_irq_key_driver",
                      (void *)keyboard_isr);

    if (ret) {
        printk(KERN_ERR "kbd_irq: Failed to register IRQ %d\n", KBD_IRQ);
        return ret;
    }
     ret = misc_register(&kbd_miscdev);
    if (ret) {
        free_irq(KBD_IRQ, (void *)keyboard_isr);
        printk(KERN_ERR "kbd_irq: misc device registration failed\n");
        return ret;
    }

    printk(KERN_INFO "kbd_irq: Keyboard IRQ registered successfully\n");
    return 0;
}

static void __exit kbd_exit(void)
{
    free_irq(KBD_IRQ, (void *)keyboard_isr);
    tasklet_kill(&td_tasklet);
    printk(KERN_INFO "kbd_irq: Keyboard IRQ driver and Tasklet unloaded\n");
}

module_init(kbd_init);
module_exit(kbd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Educational Kernel Driver");
MODULE_DESCRIPTION("Keyboard IRQ driver printing pressed keys to dmesg");
