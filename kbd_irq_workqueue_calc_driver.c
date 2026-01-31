#include <linux/module.h>   // Core module macros
#include <linux/kernel.h>   // printk()
#include <linux/init.h>     // __init, __exit
#include <linux/fs.h>       // register_chrdev, file_operations
#include <linux/uaccess.h>  // copy_to_user, copy_from_user
#include <linux/interrupt.h> //request_irq,free_irq
#include <linux/io.h>       //inb()
#include<linux/slab.h>      //kmalloc(),kfree()
#include <linux/spinlock.h>
#include<linux/workqueue.h>
#include<linux/delay.h>


static spinlock_t lock;
static struct workqueue_struct *my_wq;
static struct work_struct my_work;

#define DEVICE_NAME "kbd_irq_wq"
#define KBD_IRQ        1
#define KBD_DATA_PORT  0x60
struct st 
{
	int res;
	char strop[10];
};
static struct st *kernel_buff=NULL;
static int v1=10,v2=20;
static const char *key;
static char op;

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
static void work_handler(struct work_struct *work)
{
	pr_info("in work queue function\n");
	spin_lock(&lock);
        switch(op)
        {
                case 'A':kernel_buff->res=v1+v2;strcpy(kernel_buff->strop,"ADD");break;
                case 'S':kernel_buff->res=v1-v2; strcpy(kernel_buff->strop,"SUB");break;
                case 'M':kernel_buff->res=v1*v2; strcpy(kernel_buff->strop,"MUL");break;
                case 'D':kernel_buff->res=v2?v1/v2:0; strcpy(kernel_buff->strop,"DIV");break;
                default:spin_unlock(&lock);return;
        }
        pr_info("keyboard operation done in work queue :res:%d strop:%s\n",kernel_buff->res,kernel_buff->strop);
        spin_unlock(&lock);

}
static irqreturn_t keyboard_isr(int irq, void *dev_id)
{
    unsigned char scancode;
    bool released;
    //const char *key;
	//char op;
    scancode = inb(KBD_DATA_PORT);

    released = scancode & 0x80;
    scancode &= 0x7F;

    key = keymap[scancode];
	
    if (!key)
    {     key = "UNKNOWN";return IRQ_HANDLED;}
	op=key[0];
    if (released){
        printk(KERN_INFO "kbd_irq: Key RELEASED -> %s (scancode 0x%02x)\n",
               key, scancode);return IRQ_HANDLED;}
    else{
        printk(KERN_INFO "kbd_irq: Key PRESSED  -> %s (scancode 0x%02x)\n",
               key, scancode);}
	
	queue_work(my_wq,&my_work);
	return IRQ_HANDLED;
}


static int major_number;

/*
 * Called when user opens /dev/basic_char
 */
static int basic_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "task_calc: device opened\n");
    printk(KERN_INFO "kbd_irq: Initializing keyboard IRQ driver\n");
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
	spin_lock(&lock);
    /* user buffer size */
    bytes_to_copy = min(count, sizeof(struct st));
    struct st temp;
    temp=*kernel_buff;
    spin_unlock(&lock);
    if (copy_to_user(user_buffer,&temp, bytes_to_copy)) {
	    return -EFAULT;
    }

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
	pr_info("write function\n");
	return 4;
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
	
    printk(KERN_INFO "kbd_irq: Keyboard IRQ registered successfully\n");

    printk(KERN_INFO "kbd_irq_wq: loaded\n");
    printk(KERN_INFO "kbd_irq_wq: major number = %d\n", major_number);
    printk(KERN_INFO "Create device node with:\n");
    kernel_buff=kmalloc(sizeof(struct st),GFP_KERNEL);
    if(!kernel_buff)
	    return -ENOMEM;
    spin_lock_init(&lock);
    pr_info("wrok queue init\n");
    my_wq=create_singlethread_workqueue("superman");
    if(!my_wq)
	    return -ENOMEM;
    INIT_WORK(&my_work,work_handler);

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

    return 0;
    
}

/*
 * Module cleanup
 */
static void __exit task_calc_exit(void)
{
    free_irq(KBD_IRQ, (void *)keyboard_isr);
    flush_workqueue(my_wq);
    destroy_workqueue(my_wq);
    unregister_chrdev(major_number, DEVICE_NAME);
 	kfree(kernel_buff);
    printk(KERN_INFO "kbd_irq_wq: unloaded\n");

}

/* Kernel module macros */
module_init(task_calc_init);
module_exit(task_calc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prashant");
MODULE_DESCRIPTION("basic calc driver using keyboard irq and file operations");
