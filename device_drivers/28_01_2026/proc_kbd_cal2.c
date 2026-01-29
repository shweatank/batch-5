#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/miscdevice.h>
#include<linux/proc_fs.h>
#include<linux/uaccess.h>


#define KBD_IRQ        1
#define KBD_DATA_PORT  0x60
#define PROC_NAME "proc_kbd"
//#define BUF_LEN   128

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
struct cal
{
	char ch;
	int a;
	int b;
	int res;
};
struct cal c;
//struct cal c={3,4};
static int data_len;
static ssize_t proc_kbd_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{

	printk(KERN_INFO "in Write\n");
	data_len=count;
	//if (count != sizeof(c)){

	//printk(KERN_INFO "failed1\n");
	//	    return -EINVAL;
	if (copy_from_user(&c, buf, sizeof(c)))
	{printk(KERN_INFO "failed2\n");
        return -EFAULT;
	}
    printk(KERN_INFO "User data received: a=%d, b=%d\n", c.a, c.b);
    return count;
}

static ssize_t proc_kbd_read(struct file *file,char __user *user_buf,size_t count,loff_t *offset)
{
   /* if (*offset >= data_len)
        return 0;  // EOF

    if (count > data_len - *offset)
        count = data_len - *offset;

    if (copy_to_user(user_buf,
                  &c,sizeof(c))){
        	
    printk(KERN_INFO "read failed\n");
	    return -EFAULT;	
	}
    *offset += count;
    printk(KERN_INFO "read success\n");
    return count;*/

	return simple_read_from_buffer(user_buf,
                                   count,
                                   offset,
                                   &c,
                                   sizeof(c));
}


static const struct proc_ops proc_fops = {
    .proc_read  = proc_kbd_read,
    .proc_write = proc_kbd_write,
};



/*static const struct file_operations kbd_fops = {
    .owner = THIS_MODULE,
    .write = kbd_write,
    .read = kbd_read,
    
};*/

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
   // else
     //   printk(KERN_INFO "kbd_irq: Key PRESSED  -> %s (scancode 0x%02x)\n",
       //        key, scancode);
    if(scancode==0x1e)
    {
           c.res=c.a+c.b;
	    printk("RESULT of %d and %d with Addition is: %d \n",c.a,c.b,c.res);
    }
    else if(scancode==0x1f)
    {
           c.res=c.a-c.b;
            printk("RESULT of %d and %d with Substraction is: %d \n",c.a,c.b,c.res);
    }
    else if(scancode==0x32)
    {
           c.res=c.a*c.b;
            printk("RESULT of %d and %d with Multiplication is: %d \n",c.a,c.b,c.res);
    }
  //  else if(scancode==0x20)
   // {
     //       printk("RESULT of %d and %d with Division is: %d \n",c.a,c.b,c.a/c.b);
    //}

    return IRQ_HANDLED;
}


static int __init kbd_init(void)
{
	proc_create(PROC_NAME, 0666, NULL, &proc_fops);
   	 pr_info("proc_basic: loaded\n");

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
    return 0;
}

static void __exit kbd_exit(void)
{remove_proc_entry(PROC_NAME, NULL);
    pr_info("proc_basic: unloaded\n");

    free_irq(KBD_IRQ, (void *)keyboard_isr);
    printk(KERN_INFO "kbd_irq: Keyboard IRQ driver unloaded\n");
}

module_init(kbd_init);
module_exit(kbd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Educational Kernel Driver");
MODULE_DESCRIPTION("Keyboard IRQ driver printing pressed keys to dmesg");
