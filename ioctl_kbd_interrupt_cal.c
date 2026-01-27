#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/init.h>
#include <linux/fs.h>

#define KBD_IRQ        1
#define KBD_DATA_PORT  0x60

typedef struct st
{
	int a;
	int b;
	int res;
	char strop[20];
}st;
static st out;
static int major;
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
struct file_operations fops={
	.owner=THIS_MODULE,
	.unlocked_ioctl=basic_ioctl,
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
        printk(KERN_INFO "kbd_irq: Key PRESSED  -> %s (scancode 0x%02x)\n",
               key, scancode);
	switch(*key)
	{
		case 'A':out.res=out.a+out.b;strcpy(out.strop,"ADD");break;
		case 'S':out.res=out.a-out.b;strcpy(out.strop,"SUB");break;
		case 'M':out.res=out.a*out.b;strcpy(out.strop,"MUL");break;
		case 'D':out.res=(out.b?out.a/out.b:0);strcpy(out.strop,"DIV");break;
		default:pr_info("invalid key\n");out.res=0;strcpy(out.strop,"invaild");return -EINVAL;
	}
	pr_info("result:%d\n",out.res);
    return IRQ_HANDLED;
}
static int basic_ioctl(struct file*file,unsigned int cmd,unsigned long args)
{
	st *user_val;
	pr_info("ioctl_called\n");
	if (copy_from_user(user_val,(st __user*)args, sizeof(st)))
            return -EFAULT;
	
	out.a=user_val->a;
	out.b=user_val->b;
	return 0;
		
}
static int __init kbd_init(void)
{
    int ret;
	major=register_chrdev(0,"ioctl_cal_driver",&fops);
	if(major<0)
	{
		pr_err("failed to register %s drivre\n","ioctl_cal_driver");
		return 0;
	}
	pr_info("loaded %s with major num:%d","ioctl_cal_driver",major);
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
    return 0;
}

static void __exit kbd_exit(void)
{
	unregister_chrdev(major,"ioctl_cal_driver");
    free_irq(KBD_IRQ, (void *)keyboard_isr);
    pr_info("ioctl_cal_driver unloaded\n");
    printk(KERN_INFO "kbd_irq: Keyboard IRQ driver unloaded\n");
}

module_init(kbd_init);
module_exit(kbd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Educational Kernel Driver");
MODULE_DESCRIPTION("Keyboard IRQ driver printing pressed keys to dmesg");
