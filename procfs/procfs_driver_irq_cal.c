#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include<linux/interrupt.h>
#include<linux/io.h>
#include<linux/workqueue.h>
#include<linux/init.h>

#define PROC_NAME "proc_basic"
#define BUF_LEN   128
#define KBD_IRQ  1
#define KBD_DATA_PORT 0x60

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


static char proc_buffer[BUF_LEN];
static size_t proc_buffer_len;
struct cal
{
	int a;
	int b;
	long res;
	char ch;
};

struct cal c;
 unsigned char scancode;

static struct workqueue_struct *my_wq;
static struct work_struct my_work;

static void work_handler(struct work_struct *work)
{
	printk("in workq\n");
	    if(scancode==0x1e)
    {
            c.res=c.a+c.b;
            printk("%ld \n",c.res);
	    c.ch='+';
    }
	    if(scancode==0x1f)
    {
            c.res=c.a-c.b;
            printk("%ld \n",c.res);
	    c.ch='-';
    }
	    if(scancode==0x32)
    {
            c.res=c.a*c.b;
            printk("%ld \n",c.res);
	    c.ch='*';
    }


}
static irqreturn_t keyboard_isr(int irq,void *dev_id)
{
	const char *key;
	bool released;
	//unsigned char scancode;
    	scancode = inb(KBD_DATA_PORT);

    	released = scancode & 0x80;
    	scancode &= 0x7F;

    key = keymap[scancode];
    if (!key)
        key = "UNKNOWN";

     if (released)
        printk(KERN_INFO "kbd_irq: Key RELEASED -> %s (scancode 0x%02x)\n", key, scancode);
    else
        printk(KERN_INFO "kbd_irq: Key PRESSED  -> %s (scancode 0x%02x)\n", key, scancode);

  /*  if(scancode==0x1e)
    {
	    c.res=c.a+c.b;
	    printk("%ld \n",c.res);
    }*/
    queue_work(my_wq, &my_work);

	return IRQ_HANDLED;
}

static ssize_t proc_read(struct file *file,
                         char __user *user_buf,
                         size_t count,
                         loff_t *ppos)
{
    return simple_read_from_buffer(user_buf,
                                   count,
                                   ppos,
                                   &c,
                                   proc_buffer_len);
}

static ssize_t proc_write(struct file *file,
                          const char __user *user_buf,
                          size_t count,
                          loff_t *ppos)
{
    if (count > BUF_LEN - 1)
        count = BUF_LEN - 1;

    if (copy_from_user(&c, user_buf, count))
        return -EFAULT;

    proc_buffer[count] = '\0';
    proc_buffer_len = count;

   pr_info("proc_basic: received \"%d %d %c\"\n", c.a,c.b,c.ch);
  /*  if(c.ch=='+')
	    c.res=c.a+c.b;
    else if(c.ch=='-')
	    c.res=c.a-c.b;
    else if(c.ch=='*')
	    c.res=c.a*c.b;*/
    //int res=Separate(proc_buffer,count);
    //printk("res is %d \n",res);
  // String(proc_buffer,count,res);
   // pr_info("proc_basic: received %s\n",proc_buffer);

    return count;
}

static const struct proc_ops proc_fops = {
    .proc_read  = proc_read,
    .proc_write = proc_write,
};

static int __init proc_basic_init(void)
{
    proc_create(PROC_NAME, 0666, NULL, &proc_fops);
    pr_info("proc_basic: loaded\n");
    int ret;
    ret=request_irq(KBD_IRQ,keyboard_isr,IRQF_SHARED,"kbd_irq_key_driver",(void *)keyboard_isr);
    if (ret) {
        printk(KERN_ERR "kbd_irq: Failed to register IRQ %d\n", KBD_IRQ);
        return ret;
    }
    else
	    printk("irq is registered\n");
    my_wq=create_singlethread_workqueue("interrupt_wq");
    if(!my_wq)
	    return -ENOMEM;

    INIT_WORK(&my_work,work_handler);
//    queue_work(my_wq,&my_work);

    return 0;
}

static void __exit proc_basic_exit(void)
{
	free_irq(KBD_IRQ,(void *)keyboard_isr);
    remove_proc_entry(PROC_NAME, NULL);
    flush_workqueue(my_wq);
    destroy_workqueue(my_wq);
    pr_info("proc_basic: unloaded\n");
}

module_init(proc_basic_init);
module_exit(proc_basic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Basic procfs driver example");
