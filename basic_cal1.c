#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/init.h>     // __init, __exit
#include <linux/fs.h>       // register_chrdev, file_operations
#include <linux/uaccess.h> 

#define DEVICE_NAME "basic_char"
#define BUF_SIZE    128

static int major_number;
static char kernel_buffer[BUF_SIZE];
static int buffer_size;
static int cal;
static int n1=45,n2=87;
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
    {
       printk(KERN_INFO "kbd_irq: Key RELEASED -> %s (scancode 0x%02x)\n", key, scancode);
	if(key[0]=='A'||(key[0]=='S'&&key[1]=='\0')||key[0]=='M'||key[0]=='D')
	{
		cal=key[0];
		
	}
    }
 //   else
       // printk(KERN_INFO "kbd_irq: Key PRESSED  -> %s (scancode 0x%02x)\n",
         //      key, scancode);
	
    return IRQ_HANDLED;
}
static int basic_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "basic_char: device opened\n");
    int ret;
    printk(KERN_INFO "kbd_irq: Initializing keyboard IRQ driver\n");

    ret = request_irq(KBD_IRQ,keyboard_isr,IRQF_SHARED,"kbd_irq_key_driver",(void *)keyboard_isr);

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
    printk(KERN_INFO "basic_char: device closed\n");
    free_irq(KBD_IRQ, (void *)keyboard_isr);
    printk(KERN_INFO "kbd_irq: Keyboard IRQ driver unloaded\n");
    return 0;
}

static ssize_t basic_read(struct file *file,
                          char __user *user_buffer,
                          size_t count,
                          loff_t *offset)
{
    int bytes_to_copy;

    /*
     * If offset is beyond data, return 0 (EOF)
     */
    if (*offset >= buffer_size)
        return 0;

    bytes_to_copy = min(count, (size_t)(buffer_size - *offset));

    /*
     * Copy data from kernel space to user space
     */
    if (copy_to_user(user_buffer,
                     kernel_buffer + *offset,
                    bytes_to_copy))
        return -EFAULT;

    *offset += bytes_to_copy;

    printk(KERN_INFO "basic_char: read %d bytes\n", bytes_to_copy);
    return bytes_to_copy;
}

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
    if (copy_from_user(kernel_buffer,
                       user_buffer,
                       bytes_to_copy))
        return -EFAULT;



buffer_size = bytes_to_copy;
    //int r=calculator(kernel_buffer);
        int n1,n2,res=0;
        n1=(int )*(kernel_buffer);
        n2=(int )*(kernel_buffer+4);
 while(cal==0);
        switch(cal)
        {
                case('A'):res=n1+n2;
                          break;
                case('S'):res=n1-n2;
                          break;
                case('M'):res=n1*n2;
                          break;
                case('D'):res=n1/n2;
                          break;
                default:break;
        }
    printk(KERN_INFO "basic_char: wrote %d bytes,res=%d,n1=%d,n2=%d,ch=%c\n", bytes_to_copy,res,n1,n2,cal);
    cal=0;
    return bytes_to_copy;
}
static struct file_operations basic_fops = {
    .owner   = THIS_MODULE,
    .open    = basic_open,
    .read    = basic_read,
    .write   = basic_write,
    .release = basic_release,
};

static int __init kbd_init(void)
{
    
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

static void __exit kbd_exit(void)
{
unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "basic_char: unloaded\n");

}

module_init(kbd_init);
module_exit(kbd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Educational Kernel Driver");
MODULE_DESCRIPTION("Keyboard IRQ driver for calculator");
