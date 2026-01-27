#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include<linux/slab.h>

#define KBD_IRQ        1
#define KBD_DATA_PORT  0x60

#define DEVICE_NAME "calc_ioctl"
#define CALC_IOC_MAGIC  'C'

struct calc_req {
    int a;
    int b;
    long result;   // long to reduce overflow risk vs int (still possible)
    char s[10];       // 0 on success, -EINVAL/-ERANGE/-EDOM etc.
};


#define CALC_IOC_ADD  _IOWR(CALC_IOC_MAGIC, 1, struct calc_req)
#define CALC_IOC_SUB  _IOWR(CALC_IOC_MAGIC, 2, struct calc_req)
#define CALC_IOC_MUL  _IOWR(CALC_IOC_MAGIC, 3, struct calc_req)
#define CALC_IOC_DIV  _IOWR(CALC_IOC_MAGIC, 4, struct calc_req)
#define CALC_IOC_MOD  _IOWR(CALC_IOC_MAGIC, 5, struct calc_req)
static int major;
static struct calc_req *user_value;
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
static long calc_ioctl(struct file *file,
		unsigned int cmd,
		unsigned long arg)
{

	if (copy_from_user(user_value,(struct calc_req __user*)arg , sizeof(struct calc_req)))
         return -EFAULT;

	return 0;
}

static struct file_operations fops = {
    .owner          = THIS_MODULE,
    .unlocked_ioctl = calc_ioctl,
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
        printk(KERN_INFO "kbd_irq: Key RELEASED -> %s (scancode 0x%02x)\n",
               key, scancode);
    else
{
        printk(KERN_INFO "kbd_irq: Key PRESSED  -> %s (scancode 0x%02x)\n",
			key, scancode);
	if(*key=='A')
	{
		user_value->result=user_value->a+user_value->b;
		printk(KERN_INFO "%d + %d is=%ld\n",user_value->a,user_value->b,user_value->result);
	}
	else if(*key=='S')
	{
		user_value->result=user_value->a-user_value->b;
		printk(KERN_INFO "%d -  %d is=%ld\n",user_value->a,user_value->b,user_value->result);
	}
	else if(*key=='D')
	{
		user_value->result=user_value->a/user_value->b;
		printk(KERN_INFO "%d / %d is=%ld\n",user_value->a,user_value->b,user_value->result);
	}
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

	user_value=kmalloc(sizeof(struct calc_req),GFP_ATOMIC);
	major = register_chrdev(0, DEVICE_NAME, &fops);
	pr_info("calc_ioctl loaded, major=%d\n", major);
	printk(KERN_INFO "memory allocated\n");
	if (ret) {
		printk(KERN_ERR "kbd_irq: Failed to register IRQ %d\n", KBD_IRQ);
		return ret;
	}

    printk(KERN_INFO "kbd_irq: Keyboard IRQ registered successfully\n");
    return 0;
}

static void __exit kbd_exit(void)
{
    free_irq(KBD_IRQ, (void *)keyboard_isr);
   kfree(user_value);
    unregister_chrdev(major, DEVICE_NAME);
    pr_info("calc_ioctl unloaded\n");
  printk(KERN_INFO "memory deallocated\n");
    printk(KERN_INFO "kbd_irq: Keyboard IRQ driver unloaded\n");
}

module_init(kbd_init);
module_exit(kbd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Educational Kernel Driver");
MODULE_DESCRIPTION("Keyboard IRQ driver printing pressed keys to dmesg");
