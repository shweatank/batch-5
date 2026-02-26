#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

#define KBD_IRQ       1
#define KBD_DATA_PORT 0x60
#define DEVICE_NAME   "kbd_calc"

static int major;
static DEFINE_MUTEX(kbd_mutex);

#define MAX_BUF 128
static char input_buf[MAX_BUF];  
static int buf_index = 0;
static int result = 0;

// Simple keymap
static const char *keymap[128] = {
    [0x1E] = "A",  // Add
    [0x32] = "M",  // Multiply
    [0x1F] = "S",  // Subtract
    [0x20] = "D",  // Divide
    [0x1C] = "ENTER" // Finish input and calculate
};

// Keyboard IRQ handler
static irqreturn_t keyboard_isr(int irq, void *dev_id)
{
    unsigned char scancode;
    bool released;
    const char *key;

    scancode = inb(KBD_DATA_PORT);
    released = scancode & 0x80;
    scancode &= 0x7F;

    key = keymap[scancode];
    if (!key) return IRQ_HANDLED; // ignore other keys
    if (released) return IRQ_HANDLED;

    mutex_lock(&kbd_mutex);

    if (strcmp(key, "ENTER") == 0) {
        // Do calculation
        int op1 = 20, op2 = 2;
        char oper = 0;
        int i;

        for (i = 0; i < buf_index; i++) {
            char c = input_buf[i];
            if (c >= '0' && c <= '9') {
                if (!oper)
                    op1 = op1 * 10 + (c - '0');
                else
                    op2 = op2 * 10 + (c - '0');
            } else {
                oper = c; // store operation
            }
        }

        switch (oper) {
            case 'A': result = op1 + op2; break;
            case 'S': result = op1 - op2; break;
            case 'M': result = op1 * op2; break;
            case 'D': result = (op2 != 0) ? op1 / op2 : 0; break;
        }

        buf_index = 0; // clear buffer for next input
        printk(KERN_INFO "kbd_calc: Calculation done, result=%d\n", result);

    } else {
        // store key
        if (buf_index < MAX_BUF - 1) {
            input_buf[buf_index++] = key[0];
        }
    }

    mutex_unlock(&kbd_mutex);
    return IRQ_HANDLED;
}

// Character device read
static ssize_t dev_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
    char kbuf[64];
    int l;

    mutex_lock(&kbd_mutex);
    l = snprintf(kbuf, sizeof(kbuf), "Result: %d\n", result);
    mutex_unlock(&kbd_mutex);

    if (*offset >= l) return 0;
    if (len > l - *offset) len = l - *offset;

    if (copy_to_user(buf, kbuf + *offset, len)) return -EFAULT;
    *offset += len;

    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = dev_read,
};

static int __init kbd_calc_init(void)
{
    int ret;

    printk(KERN_INFO "kbd_calc: Loading module\n");
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ERR "kbd_calc: failed to register device\n");
        return major;
    }

    ret = request_irq(KBD_IRQ, keyboard_isr, IRQF_SHARED, "kbd_calc", (void *)keyboard_isr);
    if (ret) {
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_ERR "kbd_calc: Failed to register IRQ\n");
        return ret;
    }

    mutex_init(&kbd_mutex);
    printk(KERN_INFO "kbd_calc: Module loaded successfully, major=%d\n", major);
    return 0;
}

static void __exit kbd_calc_exit(void)
{
    free_irq(KBD_IRQ, (void *)keyboard_isr);
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "kbd_calc: Module unloaded\n");
}

module_init(kbd_calc_init);
module_exit(kbd_calc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kernel Calculator");
MODULE_DESCRIPTION("Keyboard calculator with delayed calculation (Enter to compute)");


