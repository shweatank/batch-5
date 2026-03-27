/*
 * basic_char_driver.c
 *
 * Educational Linux character device driver.
 * Demonstrates:
 *  - module init / exit
 *  - character device registration
 *  - basic file operations (open, read, write, release)
 *
 * No real hardware is accessed.
 */

#include <linux/module.h>   // Core module macros
#include <linux/kernel.h>   // printk()
#include <linux/init.h>     // __init, __exit
#include <linux/fs.h>       // register_chrdev, file_operations
#include <linux/uaccess.h>  // copy_to_user, copy_from_user
 #include <linux/interrupt.h>
#include <linux/io.h>
//#include"common.h"


#define DEVICE_NAME "basic_char"
#define BUF_SIZE    128

#define KBD_IRQ        1
#define KBD_DATA_PORT  0x60

static  char *key;

static struct var
{
	   int status;
};

struct var cal;

//extern int flag;

static int major_number;
static char k_buf[BUF_SIZE];

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

    scancode = inb(KBD_DATA_PORT);

   // released = scancode & 0x80;
   // scancode &= 0x7F;
   
    if(scancode & 0x80)
	    return IRQ_HANDLED;


    key = keymap[scancode];

    if (!key)
        key = "UNKNOWN";

       
    return IRQ_HANDLED;     // return ing the statius
}
static int basic_open(struct inode *inode, struct file *file)
{
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


    printk(KERN_INFO "basic_char: device opened\n");
    return 0;
}

/*
 * Called when user closes /dev/basic_char
 */
static int basic_release(struct inode *inode, struct file *file)
{
  free_irq(KBD_IRQ, (void *)keyboard_isr);
   printk(KERN_INFO "kbd_irq: Keyboard IRQ driver unloaded\n");

    printk(KERN_INFO "basic_char: device closed\n");
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
   
    if (copy_to_user(user_buffer,k_buf,sizeof(cal)))
        return -EFAULT;
    return count;
}

/*
 * Called when user writes to /dev/basic_char
 */
static ssize_t basic_write(struct file *file,
                           const char __user *user_buffer,
                           size_t count,
                           loff_t *offset)
{
    

    /*
     * Copy data from user space to kernel space
     */
    if (copy_from_user(k_buf,
                       user_buffer,sizeof(k_buf)))
        return -EFAULT;

    return count;
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
static int __init basic_char_init(void)
{
	 int ret;

    printk(KERN_INFO "kbd_irq: Initializing keyboard IRQ driver\n");

    printk(KERN_INFO "kbd_irq: Keyboard IRQ registered successfully\n");
    return 0;


    /*
     * Register character device
     * 0 → dynamic major number
     */
    major_number = register_chrdev(0, DEVICE_NAME, &basic_fops);
    if (major_number < 0) {
        printk(KERN_ERR "basic_char: failed to register device\n");
        return major_number;
    }

    printk(KERN_INFO "basic_char: loaded\n");
    printk(KERN_INFO "basic_char: major number = %d\n", major_number);
    printk(KERN_INFO "Create device node with:\n");
    printk(KERN_INFO "mknod /dev/%s c %d 0\n", DEVICE_NAME, major_number);

    return 0;
}

/*
 * Module cleanup
 */
static void __exit basic_char_exit(void)
{
//  free_irq(KBD_IRQ, (void *)keyboard_isr);
 //  printk(KERN_INFO "kbd_irq: Keyboard IRQ driver unloaded\n");

    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "basic_char: unloaded\n");
}

/* Kernel module macros */
module_init(basic_char_init);
module_exit(basic_char_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prashant");
MODULE_DESCRIPTION("Educational basic character driver with file operations");
