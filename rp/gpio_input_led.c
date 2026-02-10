#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>

#define DRIVER_NAME "gpio_input_led"
// BCM GPIOs (Raspberry Pi 4)
#define GPIO_LED    (512 + 17)   // GPIO17
#define GPIO_INPUT  (512 + 27)   // GPIO27

static struct task_struct *blink_thread;
static int running = 1;

/* ---------------- Kernel Thread ---------------- */
static int led_blink_thread(void *data)
{
    pr_info("LED: Blink thread started\n");

    while (!kthread_should_stop()) {

        if (gpio_get_value(GPIO_INPUT)) {
            /* Input HIGH → Blink LED */
            gpio_set_value(GPIO_LED, 1);
            msleep(500);
            gpio_set_value(GPIO_LED, 0);
            msleep(500);
        } else {
            /* Input LOW → LED OFF */
            gpio_set_value(GPIO_LED, 0);
            msleep(200);
        }
    }

    pr_info("LED: Blink thread stopping\n");
    return 0;
}

/* ---------------- Module Init ---------------- */
static int __init gpio_led_init(void)
{
    pr_info("GPIO LED Driver Init\n");
    /* Validate GPIOs */
    if (!gpio_is_valid(GPIO_LED) || !gpio_is_valid(GPIO_INPUT)) {
        pr_err("Invalid GPIOs\n");
        return -ENODEV;
    }
    /* Request LED GPIO */
    if (gpio_request(GPIO_LED, "led_gpio")) {
        pr_err("Failed to request LED GPIO\n");
        return -EBUSY;
    }
    /* Request INPUT GPIO */
    if(gpio_request(GPIO_INPUT, "input_gpio")) {
        gpio_free(GPIO_LED);
        pr_err("Failed to request INPUT GPIO\n");
        return -EBUSY;
    }

    gpio_direction_output(GPIO_LED, 0);
    gpio_direction_input(GPIO_INPUT);

    /* Start kernel thread */
    blink_thread = kthread_run(led_blink_thread, NULL, "led_blink_thread");
    if (IS_ERR(blink_thread)) {
        gpio_free(GPIO_LED);
        gpio_free(GPIO_INPUT);
        pr_err("Failed to create thread\n");
        return PTR_ERR(blink_thread);
    }

    pr_info("GPIO LED Driver Loaded\n");
    return 0;
}
/* ---------------- Module Exit ---------------- */
static void __exit gpio_led_exit(void)
{
    pr_info("GPIO LED Driver Exit\n");

    if (blink_thread)
        kthread_stop(blink_thread);

    gpio_set_value(GPIO_LED, 0);
    gpio_free(GPIO_LED);
    gpio_free(GPIO_INPUT);
}

module_init(gpio_led_init);
module_exit(gpio_led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("techdhaba");
MODULE_DESCRIPTION("GPIO Input Triggered LED Blink Driver");
