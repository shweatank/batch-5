#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include<linux/delay.h>
#include<linux/io.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Wait queue WITHOUT kthread (IRQ wakes read)");

#define DEVICE_NAME "waitq_dev"
#define IRQ_NUM 1          /* Keyboard IRQ */

static int major;

/* wait queue and condition */
static DECLARE_WAIT_QUEUE_HEAD(my_wq);
static int data_ready = 0;
static char kbuf[20] = "IRQ EVENT\n";

/* ================= READ ================= */
static ssize_t my_read(struct file *file,
                       char __user *ubuf,
                       size_t count,
                       loff_t *ppos)
{
    pr_info("read(): going to sleep\n");

    /* sleep until IRQ occurs */
    wait_event_interruptible(my_wq, data_ready == 1);

    data_ready = 0;

    if (copy_to_user(ubuf, kbuf, strlen(kbuf)))
        return -EFAULT;

    pr_info("read(): data sent to user\n");
    return strlen(kbuf);
}
static irqreturn_t my_irq_handler(int irq, void *dev_id)
{
unsigned char scancode;
scancode=inb(0x60);
bool release=scancode&0x80;

    pr_info("IRQ %d received, waking read()\n", irq);


if(scancode==0x1E)
data_ready =1;
    wake_up_interruptible(&my_wq);

    return IRQ_HANDLED;
}

static int  my_open(struct inode*inode,struct file*file)
{    
int ret = request_irq(IRQ_NUM,
                      my_irq_handler,
                      IRQF_SHARED,
                      DEVICE_NAME,
                      (void *)&major);
    if (ret) {
        unregister_chrdev(major, DEVICE_NAME);
        return ret;
    }
return 0;
}

static int my_close(struct inode*inode,struct file*file)
{
    free_irq(IRQ_NUM, (void *)&major);
pr_info("irq removed\n");
return 0;
}
/* ================= FILE OPS ================= */
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read  = my_read,
	.open=my_open,
	.release=my_close
};

/* ================= IRQ HANDLER ================= */

/* ================= INIT ================= */
static int __init waitq_init(void)
{

    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0)
        return major;


    pr_info("waitq module loaded, major=%d\n", major);
    pr_info("Create device: mknod /dev/%s c %d 0\n",
            DEVICE_NAME, major);

    return 0;
}

/* ================= EXIT ================= */
static void __exit waitq_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    pr_info("waitq module removed\n");
}

module_init(waitq_init);
module_exit(waitq_exit);

