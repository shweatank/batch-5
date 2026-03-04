#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/wait.h>
#include <linux/io.h>

#define DEVICE_NAME "key_board_file"
#define KBD_IRQ 1
#define KBD_DATA_PORT 0x60

static int major;
static wait_queue_head_t wq;
static int key9_pressed = 0;

static struct var{
	int status;
};

struct var cal;

/* IRQ handler */
#define KBD_DATA_PORT 0x60

static int ctrl_pressed = 0;

static irqreturn_t keyboard_irq_handler(int irq, void *dev_id)
{
    unsigned char scancode;

    scancode = inb(KBD_DATA_PORT);

    switch (scancode) {

        /* CTRL pressed */
        case 0x1D:
            ctrl_pressed = 1;
            break;

        /* CTRL released */
        case 0x9D:
            ctrl_pressed = 0;
            break;

        /* R pressed */
        case 0x03:
            if (ctrl_pressed) {
                printk(KERN_INFO "CTRL + R detected\n");

                key9_pressed = 1;   // your wait queue flag
                wake_up_interruptible(&wq);
            }
            break;
    }

    return IRQ_HANDLED;
}

/* Read function for user space */
static ssize_t kbd_read(struct file *file, char __user *buf,
                        size_t count, loff_t *ppos)
{
   // char message[] = "9 pressed\n";

    /* Wait until key9 is pressed */
    wait_event_interruptible(wq, key9_pressed != 0);

    cal.status=1;

    if (copy_to_user(buf, &cal, sizeof(cal)))
        return -EFAULT;

    key9_pressed = 0;  // Reset flag

    return sizeof(cal);
}

static ssize_t my_write(struct file *file,
                         const char __user *buf,
                         size_t count,
                         loff_t *ppos)
{

    /* Validate size */
    if (count != sizeof(cal))
        return -EINVAL;

    /* Copy structure from user space */
    if (copy_from_user(&cal, buf, sizeof( cal)))
        return -EFAULT;

    pr_info("status=%d\n",cal.status);

    return sizeof(cal);
}


static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read  = kbd_read,
    .write=my_write,
};

static int __init kbd_init(void)
{
    int result;

    init_waitqueue_head(&wq);

    /* Register character device */
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        pr_err("Failed to register char device\n");
        return major;
    }

    /* Request keyboard IRQ */
    result = request_irq(KBD_IRQ,
                         keyboard_irq_handler,
                         IRQF_SHARED,
                         "kbd_irq9_handler",
                         (void *)(keyboard_irq_handler));

    if (result) {
        pr_err("Cannot register IRQ 1\n");
        unregister_chrdev(major, DEVICE_NAME);
        return result;
    }

    pr_info("kbd_irq9 module loaded\n");
    pr_info("Major number: %d\n", major);
    return 0;
}

static void __exit kbd_exit(void)
{
    free_irq(KBD_IRQ, (void *)(keyboard_irq_handler));
    unregister_chrdev(major, DEVICE_NAME);
    pr_info("kbd_irq9 module unloaded\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student Example");
MODULE_DESCRIPTION("Keyboard IRQ + Wait Queue Example");

module_init(kbd_init);
module_exit(kbd_exit);

