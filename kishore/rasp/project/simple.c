#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

#define GPIO_PIN 18+512
#define GPIO_PIN 17+512

static int irq_number;

/* Tasklet function (NEW kernel format) */
static void my_tasklet_func(struct tasklet_struct *t);

/* Declare tasklet */
DECLARE_TASKLET(my_tasklet, my_tasklet_func);

/* Tasklet implementation */
static void my_tasklet_func(struct tasklet_struct *t)
{
    pr_info("Entered Tasklet\n");
}

/* ISR */
static irqreturn_t gpio_irq_handler(int irq, void *dev_id)
{
    pr_info("Entered ISR\n");

    tasklet_schedule(&my_tasklet);

    return IRQ_HANDLED;
}

/* Init */
static int __init simple_gpio_init(void)
{
    int ret;

    pr_info("Driver Loading...\n");

    ret = gpio_request(GPIO_PIN, "simple_gpio");
    if (ret){
pr_info("gpio_request failed\n");
        return ret;
}

    gpio_direction_input(GPIO_PIN);

/*    irq_number = gpio_to_irq(GPIO_PIN);
    if (irq_number < 0) {
        gpio_free(GPIO_PIN);
        return irq_number;
    }
*/
irq_number = gpio_to_irq(GPIO_PIN);
if (irq_number < 0) {
    pr_err("gpio_to_irq failed: %d\n", irq_number);
    gpio_free(GPIO_PIN);
    return -EINVAL;   // change return value temporarily
}

    ret = request_irq(irq_number,
                      gpio_irq_handler,
                      IRQF_TRIGGER_RISING,
                      "simple_gpio_irq",
                      NULL);
    if (ret) {
        gpio_free(GPIO_PIN);
        return ret;
    }

    pr_info("Driver Loaded Successfully\n");
    return 0;
}

/* Exit */
static void __exit simple_gpio_exit(void)
{
    free_irq(irq_number, NULL);
    gpio_free(GPIO_PIN);
    tasklet_kill(&my_tasklet);

    pr_info("Driver Unloaded\n");
}

module_init(simple_gpio_init);
module_exit(simple_gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Team4");
MODULE_DESCRIPTION("Simple GPIO Interrupt + Tasklet Example");

