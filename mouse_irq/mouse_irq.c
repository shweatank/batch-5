#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#define MOUSE_IRQ       40
#define MOUSE_DATA_PORT 0x60      

irqreturn_t mouse_irq_handler(int irq, void *dev_id);

irqreturn_t mouse_irq_handler(int irq, void *dev_id) {
    unsigned char data;
    data = inb(MOUSE_DATA_PORT);

    pr_info("Mouse IRQ: Data Byte = 0x%02X\n", data);

    return IRQ_HANDLED;
}

static int __init mouse_irq_init(void) {
    int result;

    pr_info("Loading custom mouse IRQ handler...\n");

    result = request_irq(MOUSE_IRQ,
                         mouse_irq_handler,
                         IRQF_SHARED,
                         "mouse_irq_handler",
                         (void *)mouse_irq_handler);

    if (result) {
        pr_err("Mouse IRQ: Cannot register IRQ %d\n", MOUSE_IRQ);
        return result;
    }

    pr_info("Mouse IRQ: Handler registered successfully\n");
    return 0;
}

static void __exit mouse_irq_exit(void) {
    free_irq(MOUSE_IRQ, (void *)mouse_irq_handler);
    pr_info("Mouse IRQ: Handler removed\n");
}

module_init(mouse_irq_init);
module_exit(mouse_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhi");
MODULE_DESCRIPTION("Simple driver for mouse interrupt");

