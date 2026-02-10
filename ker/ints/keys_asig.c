#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/io.h>

#define KBD_IRQ        1
#define KBD_DATA_PORT  0x60
#define DEVICE_NAME "ints1"

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
	int flag;
}re;


static re *r;
static int major_number;

static ssize_t basic_write(struct file *file,
                           const char __user *arg,
                           size_t count,
                           loff_t *offset)
{
  //  int bytes_to_copy;
   // bytes_to_copy = min(count, (size_t)BUF_SIZE);
     //Copy data from user space to kernel space
    if (copy_from_user(r,(re *)arg
                       ,
                       sizeof(re)));
        return -EFAULT;
	pr_info("The variables read from user are %d,%d\n",r->a,r->b);
}

static ssize_t basic_read(struct file *file,
                          char __user *user_buffer,
                          size_t count,
                          loff_t *offset)
{
   // int bytes_to_copy;

    
     // If offset is beyond data, return 0 (EOF)
     
   // if (*offset >= buffer_size)
     //  return 0;

//    bytes_to_copy = min(count, (size_t)(buffer_size - *offset));
     
	if(copy_to_user(user_buffer,
                    // kernel_buffer + *offset,
		    r,
                     //bytes_to_copy))
		     sizeof(re)))
        return -EFAULT;

return sizeof(re);
}

static void td_tasklet_fn(struct tasklet_struct *t)
{
    pr_info("td_tasklet: running cpu=%d tick=%lu\n",
            smp_processor_id(), td_tick);
}

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

   /* if (released)
        printk(KERN_INFO "kbd_irq: Key RELEASED -> %s (scancode 0x%02x)\n",
               key, scancode);
    else
        printk(KERN_INFO "kbd_irq: Key PRESSED  -> %s (scancode 0x%02x)\n",
               key, scancode);*/

    switch(*key){
	    case 'A':r->res=r->a+r->b;
		     pr_info("Sum res is %d\n",r->res);
		     break;
	case 'S':r->res=r->a-r->b;
		 if(r->res<0)
			 r->res=-(r->res);
                     pr_info("Diff res is %d\n",r->res);
                     break;
	case 'M':r->res=r->a*r->b;
                     pr_info("Mul res is %d\n",r->res);
                     break;
	case 'D':if(r->b==0)
			 r->res=0;
		 else
			 r->res=r->a/r->b;
                     pr_info("Div res is %d\n",r->res);
                     break;
	case 'O':if(r->b==0)
                         r->res=0;
                 else
                         r->res=r->a%r->b;
                     pr_info("Mod res is %d\n",r->res);
                     break;
    }
    return IRQ_HANDLED;
}

static struct file_operations basic_fops={
	.write=basic_write,
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
    return 0;
}

static void __exit kbd_exit(void)
{
    free_irq(KBD_IRQ, (void *)keyboard_isr);
    printk(KERN_INFO "kbd_irq: Keyboard IRQ driver unloaded\n");
}

module_init(kbd_init);
module_exit(kbd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Educational Kernel Driver");
MODULE_DESCRIPTION("Keyboard IRQ driver printing pressed keys to dmesg");
