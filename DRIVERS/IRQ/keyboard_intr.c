#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
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
static int num1=100,num2=200,res=0,major,*ker_buff;
static char op;
static irqreturn_t keyboard_isr(int irq, void *dev_id)
{
	unsigned char scancode;
	bool released;
	const char *key;
	scancode = inb(KBD_DATA_PORT);
	op=scancode;
	pr_info("asdfg=%d\n",op);
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
	//ker_buff->a=100,ker_buff->b=200;
	switch(op){
		case 0x1E: res=num1+num2;break;
		case 0x1F: res=num1-num2;break;
		case 0x32: res=num1*num2;break;
		case 0x20: res=num1/num2;break;
			   //	default: return IRQ_NONE;
	}


	//	printk(KERN_INFO "Result is %d\n",res);
	return IRQ_HANDLED;

}

static ssize_t my_read(struct file*file,char __user*usr,size_t size,loff_t*off){
	ker_buff=kmalloc(sizeof(int),GFP_KERNEL);
	*ker_buff=res;
	copy_to_user(usr,ker_buff,sizeof(int));
	pr_info(" 123455Result=%d\n",*ker_buff);
	kfree(ker_buff);
	return 4;
}
static ssize_t my_write(struct file*file,const char __user*usr,size_t size,loff_t *off){
	pr_info("write is called\n");
	return 4;
}

static int my_open(struct inode*inode,struct file*file){
	pr_info("opened\n");
	int ret;
	ret=request_irq(KBD_IRQ,keyboard_isr,IRQF_SHARED,"kbd_irq_key_driver",(void*)keyboard_isr);
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

	if (ret) {
		printk(KERN_ERR "kbd_irq: Failed to register IRQ %d\n", KBD_IRQ);
		return ret;
	}
	major=register_chrdev(0,"/dev/my_file",&fops);

	printk(KERN_INFO "kbd_irq: Keyboard IRQ registered successfully\n");
	return 0;
}

static void __exit kbd_exit(void)
{
	free_irq(KBD_IRQ, (void *)keyboard_isr);
	//kfree(ker_buff);
	unregister_chrdev(major,"/dev/my_file");
	printk(KERN_INFO "kbd_irq: Keyboard IRQ driver unloaded\n");
}

module_init(kbd_init);
module_exit(kbd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Educational Kernel Driver");
MODULE_DESCRIPTION("Keyboard IRQ driver printing pressed keys to dmesg");
