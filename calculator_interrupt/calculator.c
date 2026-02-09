#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/io.h>

#define KBD_IRQ       1
#define KBD_DATA_PORT 0x60

/* Minimal keyboard scan code map */
static const char *keymap[128] = {
    [0x1E] = "A",  // Addition
    [0x30] = "B",  // Subtraction
    [0x2E] = "C",  // Multiplication
    [0x20] = "D",  // Division
};

/* Hardcoded numbers for calculator operations */
static int num1 = 10;
static int num2 = 5;

static irqreturn_t keyboard_isr(int irq, void *dev_id)
{
    unsigned char scancode = inb(KBD_DATA_PORT) & 0x7F; // remove release bit
    const char *key = keymap[scancode];

    if (!key)
        return IRQ_HANDLED; // ignore unknown keys

    printk(KERN_INFO "Key PRESSED -> %s (scancode 0x%02x)\n", key, scancode);

    /* Perform operation based on key */
    switch(scancode) {
        case 0x1E: // A -> Addition
            printk(KERN_INFO "Calculator: %d + %d = %d\n", num1, num2, num1 + num2);
            break;
        case 0x30: // B -> Subtraction
            printk(KERN_INFO "Calculator: %d - %d = %d\n", num1, num2, num1 - num2);
            break;
        case 0x2E: // C -> Multiplication
            printk(KERN_INFO "Calculator: %d * %d = %d\n", num1, num2, num1 * num2);
            break;
        case 0x20: // D -> Division
            if (num2 != 0)
                printk(KERN_INFO "Calculator: %d / %d = %d\n", num1, num2, num1 / num2);
            else
                printk(KERN_INFO "Calculator: Division by zero error\n");
            break;
        default:
            break;
    }

    return IRQ_HANDLED;
}

static int __init kbd_init(void)
{
    int ret = request_irq(KBD_IRQ, keyboard_isr, IRQF_SHARED, "kbd_calc_driver", (void *)keyboard_isr);
    if (ret) {
        printk(KERN_ERR "Failed to register keyboard IRQ\n");
        return ret;
    }
    printk(KERN_INFO "Keyboard calculator driver loaded\n");
    return 0;
}

static void __exit kbd_exit(void)
{
    free_irq(KBD_IRQ, (void *)keyboard_isr);
    printk(KERN_INFO "Keyboard calculator driver unloaded\n");
}

module_init(kbd_init);
module_exit(kbd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhi");
MODULE_DESCRIPTION("Keyboard IRQ driver performing calculator operations on key press");
