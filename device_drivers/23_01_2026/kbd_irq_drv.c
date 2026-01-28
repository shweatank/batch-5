#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include<linux/slab.h>
#include<linux/gfp.h>


#define KBD_IRQ        1
#define KBD_DATA_PORT  0x60
#define DEVICE_NAME "irq_cal"
#define BUF_SIZE 256

static dev_t dev_num;
static struct cdev rev_cdev;

static char kernel_buf[BUF_SIZE];
static int data_len;

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
struct st{
        int a;
        int b;
};
static struct st *v;

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

    return IRQ_HANDLED;
}
static int rev_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "reverse_dev: opened\n");
    return 0;
}

static int rev_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "reverse_dev: closed\n");
    return 0;
}

static ssize_t rev_write(struct file *file,
                         const struct st __user *user_buf,
                         size_t count,
                         loff_t *offset)
{
    if (count >= BUF_SIZE)
        count = BUF_SIZE - 1;

    if (copy_from_user(v, user_buf, count))
        return -EFAULT;



    printk(KERN_INFO "reverse_dev: received & reversed string\n");
    return count;
}
static ssize_t rev_read(struct file *file,
                        struct st __user *user_buf,
                        size_t count,
                        loff_t *offset)
{
    if (*offset >= data_len)
        return 0;  // EOF

    if (count > data_len - *offset)
        count = data_len - *offset;

    if (copy_to_user(user_buf,
                     v,
                     count))
        return -EFAULT;

    *offset += count;
    return count;
}

static int __init kbd_init(void)
{
    int ret;

    printk(KERN_INFO "kbd_irq: Initializing keyboard IRQ driver\n");
	v=kmalloc(sizeof(*v),GFP_ATOMIC);
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
