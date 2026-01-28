#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include<linux/delay.h>



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
static int a=100,b=200,res=0,major,*ker_buff;
static char op;
static struct workqueue_struct *my_wq;
static struct work_struct irq_work;

static void work_queue_handler(struct work_struct *work){
	pr_info("Entered work queue handler\n");
	msleep(1);
	int i;
	for(i=2;i<=res;i++)
		if(res%i==0)
			break;
	if(i==res)
		pr_info("Result is prime\n");
	else
		pr_info("Result is not prime\n");
}

static void td_tasklet_fn(struct tasklet_struct *t){
	pr_info("Entered tasklet function\n");
	switch(op){
		case 0x1E:res=a+b;break;
		case 0x1F:res=a-b;break;
		case 0x32:res=a*b;break;
		case 0x20:res=a/b;break;
	}
	pr_info("Result=%d\n",res);
	queue_work(my_wq,&irq_work);
}

DECLARE_TASKLET(td_tasklet,td_tasklet_fn);

static irqreturn_t keyboard_isr(int irq, void *dev_id)
{
	unsigned char scancode;
	bool released;
	const char *key;
	scancode = inb(KBD_DATA_PORT);
	op=scancode;
	released = scancode & 0x80;
	scancode &= 0x7F;
	key = keymap[scancode];

	if (!key)
		key = "UNKNOWN";

	if (released)
		printk(KERN_INFO "kbd_irq: Key RELEASED -> %s (scancode 0x%02x)\n",
				key, scancode);
	else
		printk(KERN_INFO "kbd_irq: Key PRESSED  -> %s (scancode 0x%02x)\n",
				key, scancode);
	tasklet_schedule(&td_tasklet);

	return IRQ_HANDLED;

}
static ssize_t my_read(struct file*file,char __user*usr,size_t size,loff_t*off){
	ker_buff=kmalloc(sizeof(int),GFP_KERNEL);
	*ker_buff=res;
	if(*off>=sizeof(int))
		return 0;
	/*	if(size>sizeof(int))
		size=sizeof(int);
	 */
	size=4;
	ssize_t ret;
	ret=copy_to_user(usr,ker_buff,sizeof(int));
	if(ret!=0)
		return -EFAULT;
	*off+=size;
	pr_info(" 123455Result=%d\n",*ker_buff);
	kfree(ker_buff);
	return size;
}
static ssize_t my_write(struct file*file,const char __user*usr,size_t size,loff_t *off){
	pr_info("write is called\n");
	return 4;
}

static int my_open(struct inode*inode,struct file*file){
	pr_info("opened\n");
	int ret;
//	ret=request_irq(KBD_IRQ,keyboard_isr,IRQF_SHARED,"kbd_irq_key_driver",(void*)keyboard_isr);
	return 0;
}
static struct file_operations fops={
	.owner=THIS_MODULE,
	.open=my_open,
	.read=my_read,
	.write=my_write,
};

static int __init kbd_init(void)
{
	int ret;
	//ker_buff=kmalloc(sizeof(struct st),GFP_KERNEL);

	printk(KERN_INFO "kbd_irq: Initializing keyboard IRQ driver\n");

	ret = request_irq(KBD_IRQ,
			keyboard_isr,
			IRQF_SHARED,
			"kbd_irq_key_driver",
			(void *)keyboard_isr);
	//	tasklet_schedule(&td_tasklet);
	if (ret) {
		printk(KERN_ERR "kbd_irq: Failed to register IRQ %d\n", KBD_IRQ);
		return ret;
	}
	major=register_chrdev(0,"/dev/my_file",&fops);
	printk(KERN_INFO "kbd_irq: Keyboard IRQ registered successfully, major number is %d\n",major);

	my_wq=create_singlethread_workqueue("irq_wq");
	INIT_WORK(&irq_work,work_queue_handler);

	return 0;
}

static void __exit kbd_exit(void)
{
	free_irq(KBD_IRQ, (void *)keyboard_isr);
	//kfree(ker_buff);
	tasklet_kill(&td_tasklet);
	unregister_chrdev(major,"/dev/my_file");
	printk(KERN_INFO "kbd_irq: Keyboard IRQ driver unloaded\n");

//	free_irq(irq,(void*)&irq);
	flush_workqueue(my_wq);
	destroy_workqueue(my_wq);

}

module_init(kbd_init);
module_exit(kbd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Educational Kernel Driver");
MODULE_DESCRIPTION("Keyboard IRQ driver printing pressed keys to dmesg");
