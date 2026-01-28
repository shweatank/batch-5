#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include<linux/interrupt.h>
#include<linux/io.h>
#include<linux/delay.h>
#include<linux/workqueue.h>

#define PROC_NAME "proc_basic"
#define BUF_LEN   128
#define KBD_IRQ        1
#define KBD_DATA_PORT  0x60

static struct workqueue_struct *my_wq;
static struct work_struct irq_work;

struct calc
{
int a;
int b;
int res;
};

unsigned char scancode;
struct calc *k_buf;
static char proc_buffer[BUF_LEN];
static size_t proc_buffer_len;

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






static ssize_t proc_read(struct file *file,
                         char __user *user_buf,
                         size_t count,
                         loff_t *ppos)
{

printk(KERN_INFO "in read\n");

if(count>sizeof(struct calc))
return -EFAULT;

if(copy_to_user(user_buf,k_buf,sizeof(struct calc)))
return -EFAULT;
printk(KERN_INFO "res=%d\n",k_buf->res);
return sizeof(struct calc);
}

static void work_handler(struct work_struct *work)
{
pr_info("in work handler\n");    
      switch(scancode)
{
case 0x1E:
     k_buf->res=k_buf->a+k_buf->b;  
     break;
case 0x1F:
     k_buf->res=k_buf->a-k_buf->b;  
     break;
case 0x32:
     k_buf->res=k_buf->a*k_buf->b;  
     break;
case 0x20:
     k_buf->res=k_buf->a/k_buf->b;  
     break;
case 0x18:
     k_buf->res=k_buf->a%k_buf->b;  
     break;
}
pr_info("proc_basic:=%d\n", k_buf->res);
}

static irqreturn_t keyboard_isr(int irq,void *dev_id)
{
scancode=inb(KBD_DATA_PORT);
const char *key;
bool release;
release=scancode&0x80;
scancode&=0x7f;
key=keymap[scancode];

if(!release)
pr_info("%s is PRESSED with scancode %02x",key,scancode);

queue_work(my_wq,&irq_work);
return IRQ_HANDLED;
}

static ssize_t proc_write(struct file *file,
                          const char __user *user_buf,
                          size_t count,
                          loff_t *ppos)
{
struct calc *k;
    if (count != sizeof(struct calc))
        return -EFAULT;

    if (copy_from_user(proc_buffer, user_buf, count))
        return -EFAULT;

    k=(struct calc*)proc_buffer;
    k_buf->a=k->a;
    k_buf->b=k->b;
pr_info("in write\n");
    return count;
}
static int proc_open(struct inode*inode,struct file*file)
{

	int ret=request_irq(KBD_IRQ,keyboard_isr,IRQF_SHARED,"my_keyboard",(void*)keyboard_isr);
		if(ret)
			pr_info("keyboard interrupt failed\n");
	pr_info("keybord irq initialized\n");
	my_wq=create_singlethread_workqueue("work queue");
	INIT_WORK(&irq_work,work_handler);
	pr_info("work queue initialized\n");

return 0;
}
static int proc_release(struct inode*inode,struct file*file)
{

return 0;
}

static const struct proc_ops proc_fops = {
    .proc_read  = proc_read,
    .proc_write = proc_write,
    .proc_open  =proc_open,
    .proc_release=proc_release
};

static int __init proc_basic_init(void)
{
	proc_create(PROC_NAME, 0666, NULL, &proc_fops);
        k_buf=kmalloc(sizeof(struct calc),GFP_ATOMIC);
	pr_info("proc_basic: loaded\n");
	return 0;
}

static void __exit proc_basic_exit(void)
{
	remove_proc_entry(PROC_NAME, NULL);
	pr_info("proc_basic: unloaded\n");
	free_irq(KBD_IRQ,(void*)keyboard_isr);
	pr_info("keyboard freed\n");  
	flush_workqueue(my_wq);
	destroy_workqueue(my_wq);
	pr_info("work queue flushed\n"); 
	kfree(k_buf);
}


module_init(proc_basic_init);
module_exit(proc_basic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SAI KUMAR");
MODULE_DESCRIPTION("Basic work queue with proc");
