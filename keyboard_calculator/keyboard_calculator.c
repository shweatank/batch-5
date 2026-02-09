#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/keyboard.h>
#include <linux/irq.h>
#include <linux/irqreturn.h>
#include <linux/init.h>

static struct tasklet_struct calc_tasklet;
static char last_key = 0;

static int num1 = 10;
static int num2 = 5;


static void calc_tasklet_fn(unsigned long data)
{
    int result;

    switch (last_key) {
        case 'a': // Addition
            result = num1 + num2;
            pr_info("Calculator: %d + %d = %d\n", num1, num2, result);
            break;
        case 'b': // Subtraction
            result = num1 - num2;
            pr_info("Calculator: %d - %d = %d\n", num1, num2, result);
            break;
        case 'c': // Multiplication
            result = num1 * num2;
            pr_info("Calculator: %d * %d = %d\n", num1, num2, result);
            break;
        case 'd': // Division
            if (num2 != 0) {
                result = num1 / num2;
                pr_info("Calculator: %d / %d = %d\n", num1, num2, result);
            } else {
                pr_info("Calculator: Division by zero!\n");
            }
            break;
        default:
            break; 
    }
}


static irqreturn_t kb_irq_handler(int irq, void *dev_id)
{
    unsigned char scancode;
    scancode = inb(0x60);  

    
    if (!(scancode & 0x80)) {
        switch (scancode) {
            case 0x1e: last_key = 'a'; break; // 'A' key
            case 0x30: last_key = 'b'; break; // 'B' key
            case 0x2e: last_key = 'c'; break; // 'C' key
            case 0x20: last_key = 'd'; break; // 'D' key
            default:
                last_key = 0; // ignore other keys
                break;
        }

        if (last_key)
            tasklet_schedule(&calc_tasklet);
    }

    return IRQ_HANDLED;
}

static int irq_number = 1;


static int __init calc_init(void)
{
    int ret;

    pr_info("Calculator Tasklet Module: Init\n");

    
    tasklet_init(&calc_tasklet, calc_tasklet_fn, 0);

    
    ret = request_irq(irq_number, kb_irq_handler, IRQF_SHARED, "calc_tasklet_irq", (void *)(kb_irq_handler));
    if (ret) {
        pr_err("Calculator Tasklet: Failed to request IRQ %d\n", irq_number);
        tasklet_kill(&calc_tasklet);
        return ret;
    }

    pr_info("Calculator Tasklet: IRQ %d registered\n", irq_number);
    return 0;
}

static void __exit calc_exit(void)
{
    free_irq(irq_number, (void *)(kb_irq_handler));
    tasklet_kill(&calc_tasklet);
    pr_info("Calculator Tasklet Module: Exit\n");
}

module_init(calc_init);
module_exit(calc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhi");
MODULE_DESCRIPTION("Keyboard tasklet calculator demo");
MODULE_VERSION("1.0");

