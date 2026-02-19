#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

static int gpio_num;
static int irq_number;

/* ISR */
static irqreturn_t gpio_isr(int irq, void *dev_id)
{
    pr_info("GPIO17 ISR Triggered!\n");
    return IRQ_HANDLED;
}

/* Init */
static int __init gpio17_init(void)
{
    struct device_node *np;
    int ret;

    pr_info("GPIO17 ISR Driver Loading...\n");

    /* Find /gpio node in device tree */
    np = of_find_node_by_path("/gpio");
    if (!np) {
        pr_err("Cannot find /gpio node\n");
        return -ENODEV;
    }

    /* Get GPIO by label defined in DTS */
    gpio_num = of_get_named_gpio(np, "isr_gpio17", 0);
    if (!gpio_is_valid(gpio_num)) {
        pr_err("Invalid GPIO from DT: %d\n", gpio_num);
        return -ENODEV;
    }

    pr_info("GPIO from DT: %d\n", gpio_num);

    /* Request GPIO */
    ret = gpio_request(gpio_num, "gpio17_isr");
    if (ret) {
        pr_err("gpio_request failed: %d\n", ret);
        return ret;
    }

    gpio_direction_input(gpio_num);

    /* Map GPIO to IRQ */
    irq_number = gpio_to_irq(gpio_num);
    if (irq_number < 0) {
        pr_err("gpio_to_irq failed: %d\n", irq_number);
        gpio_free(gpio_num);
        return irq_number;
    }

    /* Request IRQ on rising edge */
    ret = request_irq(irq_number, gpio_isr, IRQF_TRIGGER_RISING,
                      "gpio17_isr_irq", NULL);
    if (ret) {
        pr_err("request_irq failed: %d\n", ret);
        gpio_free(gpio_num);
        return ret;
    }

    pr_info("GPIO17 ISR Driver Loaded\n");
    return 0;
}

/* Exit */
static void __exit gpio17_exit(void)
{
    free_irq(irq_number, NULL);
    gpio_free(gpio_num);
    pr_info("GPIO17 ISR Driver Unloaded\n");
}

module_init(gpio17_init);
module_exit(gpio17_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Team4");
MODULE_DESCRIPTION("GPIO17 ISR Driver using Device Tree Overlay");

