#include <linux/module.h>   // Core module macros
#include <linux/kernel.h>   // printk()
#include <linux/init.h>     // __init, __exit
#include <linux/fs.h>       // register_chrdev, file_operations
#include <linux/uaccess.h>  // copy_to_user, copy_from_user
#include <linux/interrupt.h>
#include <linux/io.h>
#include<linux/spinlock.h>

#define DEVICE_NAME "task_calc"
#define BUF_SIZE    128
#define KBD_IRQ        1
#define KBD_DATA_PORT  0x60

struct st
{
        int a;
        int b;
        int res;
	int done;
        char strop[10];
};

/* completion object */
static DECLARE_COMPLETION(result_ready);

spinlock_t lock;
static struct st cal_var;
static const char *key;
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
static irqreturn_t keyboard_isr(int irq, void *dev_id)
{
    unsigned char scancode;
    bool released;
    //const char *key;

    scancode = inb(KBD_DATA_PORT);

    released = scancode & 0x80;
    scancode &= 0x7F;

    key = keymap[scancode];

    /*if (!key)
        key = "UNKNOWN";

    if (released)
        printk(KERN_INFO "kbd_irq: Key RELEASED -> %s (scancode 0x%02x)\n",
               key, scancode);
    else
        printk(KERN_INFO "kbd_irq: Key PRESSED  -> %s (scancode 0x%02x)\n",
               key, scancode);
*/
    	spin_lock(&lock);
	switch(*key)
	{
		case 'A':cal_var.res=cal_var.a+cal_var.b; strcpy((cal_var.strop),"ADD");break;
		case 'S':cal_var.res=cal_var.a-cal_var.b; strcpy((cal_var.strop),"SUB");break;
		case 'M':cal_var.res=cal_var.a*cal_var.b; strcpy((cal_var.strop),"MUL");break;
		case 'D':cal_var.res=cal_var.b?cal_var.a/cal_var.b:0; strcpy((cal_var.strop),"DIV");break;
		default:pr_info("invalid key\n");spin_unlock(&lock);return IRQ_HANDLED;
	}
	cal_var.done=1;
	spin_unlock(&lock);
	complete(&result_ready);
	pr_info("keyboard operation done:res:%d strop:%s\n",cal_var.res,cal_var.strop);
    return IRQ_HANDLED;
}



static int major_number;
static struct st kernel_buffer;
static int buffer_size;

/*
 * Called when user opens /dev/basic_char
 */
static int basic_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "task_calc: device opened\n");
    printk(KERN_INFO "kbd_irq: Initializing keyboard IRQ driver\n");
    
    spin_lock_init(&lock);
    int ret;
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

/*
 * Called when user closes /dev/basic_char
 */
static int basic_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "task_calc: device closed\n");
    return 0;
}

/*
 * Called when user reads from /dev/basic_char
 */
static ssize_t basic_read(struct file *file,
                          char __user *user_buffer,
                          size_t count,
                          loff_t *offset)
{
//	size_t buffer_size = sizeof(struct st);
    size_t bytes_to_copy;
	 
    /* sleep until keyboard ISR runs */
    if (wait_for_completion_interruptible(&result_ready))
        return -ERESTARTSYS;

    spin_lock(&lock);

    if (!cal_var.done) {
        spin_unlock(&lock);
	pr_info("task not yet done\n");
        return -EIO;  /* should not happen */
    }

     /* Respect the user buffer size */
    bytes_to_copy = min(count, sizeof(struct st));
    
        spin_unlock(&lock);
    /* Copy the struct to user space */
    if (copy_to_user(user_buffer, &cal_var, bytes_to_copy)) {
        return -EFAULT;
    }

    /* Do NOT update offset since we are ignoring it */
    printk(KERN_INFO "task_calc: read %zu bytes\n", bytes_to_copy);
    return bytes_to_copy;
}
/*
 * Called when user writes to /dev/basic_char
 */
static ssize_t basic_write(struct file *file,
                           const char __user *user_buffer,
                           size_t count,
                           loff_t *offset)
{
    int bytes_to_copy;

    bytes_to_copy = min(count, (size_t)BUF_SIZE);
	
    /*
     * Copy data from user space to kernel space
     */
    if (copy_from_user(&kernel_buffer,
                       user_buffer,
                       bytes_to_copy))
        return -EFAULT;
    spin_lock(&lock);
    cal_var.a=kernel_buffer.a;
    cal_var.b=kernel_buffer.b;
    cal_var.done=0;
	bytes_to_copy=sizeof(struct st);
	pr_info("written %d bytes\n",bytes_to_copy);
	spin_unlock(&lock);
	reinit_completion(&result_ready);
    return bytes_to_copy;
}

/*
 * File operations structure
 * This connects system calls to driver functions
 */
static struct file_operations basic_fops = {
    .owner   = THIS_MODULE,
    .open    = basic_open,
    .read    = basic_read,
    .write   = basic_write,
    .release = basic_release,
};

/*
 * Module initialization
 */
static int __init task_calc_init(void)
{
    /*
     * Register character device
     * 0 → dynamic major number
     */
    major_number = register_chrdev(0, DEVICE_NAME, &basic_fops);
    if (major_number < 0) {
        printk(KERN_ERR "task_calc: failed to register device\n");
        return major_number;
    }

    printk(KERN_INFO "task_calc: loaded\n");
    printk(KERN_INFO "task_calc: major number = %d\n", major_number);
    printk(KERN_INFO "Create device node with:\n");
    return 0;
    
}

/*
 * Module cleanup
 */
static void __exit task_calc_exit(void)
{
    unregister_chrdev(major_number, DEVICE_NAME);
    free_irq(KBD_IRQ, (void *)keyboard_isr);
    printk(KERN_INFO "task_calc: unloaded\n");

}

/* Kernel module macros */
module_init(task_calc_init);
module_exit(task_calc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prashant");
MODULE_DESCRIPTION("basic calc driver using keyboard irq and file operations");
