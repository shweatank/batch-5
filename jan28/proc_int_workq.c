#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/interrupt.h>
#include<linux/io.h>
#include<linux/proc_fs.h>
#include<linux/uaccess.h>
#include<linux/delay.h>
#include<linux/workqueue.h>
#include<linux/init.h>

#define PROC_NAME	"proc"
#define BUF_LEN		128
#define KBD_IRQ		1
#define KBD_DATA_PORT	0X60

static struct workqueue_struct *my_wq;
static struct work_struct my_work;

const char *key;
unsigned char scancode;
struct d
{
	int n1,n2;
};
struct d data;
static int result;
static char ch;
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


static void work_handler(struct work_struct *work)
{
	ch=key[0];
	pr_info("workq handler\n");
	pr_info("key %c \n",ch);
	switch(ch)
	{
		case 'A':result=data.n1+data.n2;
			 pr_info("result : %d\n",result);
			 break;
		case 'S':result=data.n1-data.n2;
			 break;
		case 'M':result=data.n1*data.n2;
			 break;
		case 'D':if(data.n2!=0)
				 result=data.n1/data.n2;
			 break;
	}
	pr_info("workq handler exit\n");
}


static irqreturn_t keyboard_isr(int irq, void *dev_id)
{
    bool released;

    scancode = inb(KBD_DATA_PORT);

    released = scancode & 0x80;
    scancode &= 0x7F;

    key = keymap[scancode];
    if(!key)
	    return IRQ_HANDLED;
    if (released)
    {
        printk(KERN_INFO "kbd_irq: Key RELEASED -> %s (scancode 0x%02x)\n",
               key, scancode);
    }
    else
        printk(KERN_INFO "kbd_irq: Key PRESSED  -> %s (scancode 0x%02x)\n",
               key, scancode);

    queue_work(my_wq,&my_work);

    return IRQ_HANDLED;
}

static ssize_t proc_read(struct file *file,
                         char __user *user_buf,
                         size_t count,
                         loff_t *ppos)
{
 /*   size_t available;

    if (*ppos > 0)
        return 0;

    available = proc_buffer_len;
    if (count < available)
        available = count;
*/
    if (copy_to_user(user_buf, &result, sizeof(result)))
        return -EFAULT;

    //*ppos += available;
    //return available;
    return sizeof(result);
}


static ssize_t proc_write(struct file *file,
                          const char __user *user_buf,
                          size_t count,
                          loff_t *ppos)
{

    if (copy_from_user(&data, user_buf, sizeof(data)))
        return -EFAULT;
        
    pr_info("proc_basic: received n1=%d n2=%d\n", data.n1,data.n2);
    return sizeof(data);
}
static const struct proc_ops proc_fops = {
    .proc_read  = proc_read,
    .proc_write = proc_write,
};


static int __init proc_basic_init(void)
{
    proc_create(PROC_NAME, 0666, NULL, &proc_fops);
    int ret=request_irq(KBD_IRQ,
		    keyboard_isr,
		    IRQF_SHARED,
		    "kbd_irq_key_proc_int",
		    (void *)keyboard_isr);
if(ret)
	return -EFAULT;
    my_wq = create_singlethread_workqueue("proc_int_wq");
    INIT_WORK(&my_work,work_handler);
    pr_info("proc_basic: loaded\n");
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
MODULE_DESCRIPTION("Basic procfs driver with copy_to_user");
