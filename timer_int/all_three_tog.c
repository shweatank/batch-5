#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/io.h>


#define KBD_IRQ        1
#define KBD_DATA_PORT  0x60
#define DEVICE_NAME "irq_bh"

/* Simple US keyboard scan code map (Set 1) */
static const char *keymap[128] = {
 //   [0x01] = "ESC",
  //  [0x02] = "1",   [0x03] = "2",   [0x04] = "3",
  //  [0x05] = "4",   [0x06] = "5",   [0x07] = "6",
  //  [0x08] = "7",   [0x09] = "8",   [0x0A] = "9",
  //  [0x0B] = "0",
  //  [0x0C] = "-",   [0x0D] = "=",
  //  [0x0E] = "BACKSPACE",
   // [0x0F] = "TAB",
  //  [0x10] = "Q",   [0x11] = "W",   [0x12] = "E",
  //  [0x13] = "R",   [0x14] = "T",   [0x15] = "Y",
  //  [0x16] = "U",   [0x17] = "I",  
  [0x18] = "O",
  //  [0x19] = "P",
  //  [0x1C] = "ENTER",
  //  [0x1D] = "CTRL",
    [0x1E] = "A",   [0x1F] = "S",   [0x20] = "D",
  //  [0x21] = "F",   [0x22] = "G",   [0x23] = "H",
  //  [0x24] = "J",   [0x25] = "K",   [0x26] = "L",
  //  [0x2C] = "Z",   [0x2D] = "X",   [0x2E] = "C",
  //  [0x2F] = "V",   [0x30] = "B",   [0x31] = "N",
    [0x32] = "M",
  //  [0x39] = "SPACE"
};

typedef struct result{
	int a;
	int b;
	int res;
}re;

static char kbuff[10];
static re *r;
static int major_number;
static int a=20,b=10,rs;
static char *ch;
static struct workqueue_struct *my_wq;
static struct work_struct irq_work;
static DECLARE_WAIT_QUEUE_HEAD(my_waitq);
static int flag=0;
static struct task_struct *worker_thread;

/*static int basic_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "ints: device opened\n");
    return 0;
}*/

static ssize_t basic_write(struct file *file,
                           const char __user *arg,
                           size_t count,
                           loff_t *offset)
{
	size_t n;
	n=(count<sizeof(re))?count:sizeof(re);
	
	//if(copy_from_user(r,(re *)arg,sizeof(re))
    if (copy_from_user(r,(re *)arg,n))
        return -EFAULT;
	pr_info("The variables read from user are %d,%d\n",r->a,r->b);
return sizeof(re);
}

static ssize_t basic_read(struct file *file,
                          char __user *user_buffer,
                          size_t count,
                          loff_t *offset)
{ 
	int bytes_to_copy;
	if(*offset>=sizeof(re))
		return 0;
	//bytes_to_copy=min(count,(size_t)(sizeof(kbuff)-*offset));
//	if(flag==1){
	 wait_event_interruptible(my_waitq,flag==1);
	bytes_to_copy=min(count,sizeof(re)-*offset);
	//wait_event_interruptible(my_waitq,flag==1);
		if(copy_to_user((re *)user_buffer, r,bytes_to_copy))
        		return -EFAULT;
	flag=0;
	*offset+=bytes_to_copy;

return bytes_to_copy;
}

/*tatic int wait_thread_fn(void *data)
{
    pr_info("wait_thread: started, waiting for input from work queue\n");

    while (!kthread_should_stop()) {

         Sleep until IRQ signals event 
        wait_event_interruptible(my_waitq, flag==1);

         Reset condition 
        flag = 0;

        pr_info("wait_thread: woke up, processing data\n");

         Heavy work allowed 
        msleep(1000);

        pr_info("wait_thread: work done\n");
    }

    return 0;
}*/

static void work_handler(struct work_struct *work)
{
    pr_info("work_handler: running in process context\n");
	switch(*ch){
            case 'A':rs=a+b;
                     pr_info("Sum res is %d\n",rs);
                     break;
        case 'S':rs=a-b;
                 if(rs<0)
                         rs=-(rs);
                     pr_info("Diff res is %d\n",rs);
                     break;
        case 'M':rs=a*b;
                     pr_info("Mul res is %d\n",rs);
                     break;
        case 'D':if(b==0)
                         rs=0;
                 else
                         rs=a/b;
                     pr_info("Div res is %d\n",rs);
                     break;
        case 'O':if(b==0)
                         rs=0;
                 else
                         rs=a%b;
                     pr_info("Mod res is %d\n",rs);
                     break;
    }
    /* Heavy work is allowed */
   // msleep(1000);
	
    flag=1;
 wake_up_interruptible(&my_waitq);
    pr_info("work_handler: completed\n");
}




static void td_tasklet_fn(struct tasklet_struct *t)
{
	pr_info("in tasklet\n");
	pr_info("pressed %c\n",*ch);
	queue_work(my_wq,&irq_work);
	//pr_info("IN TASKLET : Res is %d\n",rs);
}

DECLARE_TASKLET(td_tasklet, td_tasklet_fn);



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
    ch=key;
	tasklet_schedule(&td_tasklet);
    return IRQ_HANDLED;
}

static struct file_operations basic_fops={
	.write=basic_write,
	.read=basic_read,
//	.open=basic_open,
};



static int __init kbd_init(void)
{
	 major_number = register_chrdev(0, DEVICE_NAME, &basic_fops);
    if (major_number < 0) {
        printk(KERN_ERR "basic_char: failed to register device\n");
        return major_number;
    }
	 printk(KERN_INFO "Create device node with:\n");
    printk(KERN_INFO "mknod /dev/%s c %d 0\n", DEVICE_NAME, major_number);
	r=kmalloc(sizeof(re),GFP_ATOMIC);
	r->a=0;
	r->b=0;
	r->res=0;
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

    printk(KERN_INFO "kbd_irq: Keyboard IRQ registered successfully\n");
    
   pr_info("wq_init\n");
    my_wq = create_singlethread_workqueue("irq_wq");
    if(!my_wq)
	    return -ENOMEM;
    INIT_WORK(&irq_work,work_handler);
    
   // pr_info("irq_waitq\n");
   // worker_thread=kthread_run(wait_thread_fn,NULL,"workq_wait_thread");
    //if(IS_ERR(worker_thread))
//	    return PTR_ERR(worker_thread);
    return 0;
}

static void __exit kbd_exit(void)
{
    free_irq(KBD_IRQ, (void *)keyboard_isr);
    pr_info("td:tasklet : exit\n");
    tasklet_kill(&td_tasklet);
    pr_info("td_tasklet: done\n");
    pr_info("wq exit\n");
    flush_workqueue(my_wq);
    destroy_workqueue(my_wq);
    printk(KERN_INFO "kbd_irq: Keyboard IRQ driver unloaded\n");
  //  pr_info("waitq exit\n");
  //  flag=1;
  //  wake_up_interruptible(&my_waitq);
  //  kthread_stop(worker_thread);
}

module_init(kbd_init);
module_exit(kbd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Educational Kernel Driver");
MODULE_DESCRIPTION("Keyboard IRQ driver printing pressed keys to dmesg");
