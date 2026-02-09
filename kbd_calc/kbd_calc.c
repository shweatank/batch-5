#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/io.h>

#define KBD_IRQ       1
#define KBD_DATA_PORT 0x60

/* Hardcoded numbers */
static int num1 = 10;
static int num2 = 5;

static irqreturn_t keyboard_isr(int irq, void *dev_id)
{
    unsigned char scancode;

    scancode = inb(KBD_DATA_PORT) & 0x7F;  // mask release bit

    switch (scancode) {
    case 0x1E:  // A key → Addition
        printk(KERN_INFO "A pressed: %d + %d = %d\n",
               num1, num2, num1 + num2);
        break;

    case 0x30:  // B key → Subtraction
        printk(KERN_INFO "B pressed: %d - %d = %d\n",
               num1, num2, num1 - num2);
        break;

    case 0x2E:  // C key → Multiplication
        printk(KERN_INFO "C pressed: %d * %d = %d\n",
               num1, num2, num1 * num2);
        break;

    case 0x20:  // D key → Division
        if (num2 != 0)
            printk(KERN_INFO "D pressed: %d / %d = %d\n",
                   num1, num2, num1 / num2);
        else
            printk(KERN_INFO "Division by zero error\n");
        break;

    default:
        break;
    }

    return IRQ_HANDLED;
}

static int __init kbd_init(void)
{
    int ret;

    ret = request_irq(KBD_IRQ,
                      keyboard_isr,
                      IRQF_SHARED,
                      "kbd_calc_driver",
                      (void *)keyboard_isr);

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
MODULE_DESCRIPTION("Keyboard IRQ calculator");
