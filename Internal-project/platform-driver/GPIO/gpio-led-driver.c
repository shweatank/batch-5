#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/delay.h>

static struct gpio_desc *gpio;

static int gpio_probe_func(struct platform_device *p)
{
    gpio = devm_gpiod_get(&p->dev, "led", GPIOD_OUT_LOW);
    if (IS_ERR(led_gpio)) {
        pr_err("GPIO  failed\n");
        return PTR_ERR(led_gpio);
    }

    pr_info("5 times led blink\n");
    for (int i = 0; i < 5; i++) {
        gpiod_set_value(led_gpio, 1);
        msleep(1000);
        gpiod_set_value(led_gpio, 0);
        msleep(1000);
    }
    return 0;
}

static int gpio_remove_func(struct platform_device *pdev)
{
    if (led_gpio)
        gpiod_set_value(led_gpio, 0);
    pr_info("GPIO LED removed\n");
    return 0;
}

static const struct of_device_id gpio_led_match[] = {
    { .compatible = "gpio,led" },
    {}
};
MODULE_DEVICE_TABLE(of, gpio_led__match);

static struct platform_driver gpio_led_driver = {
    .probe = gpio_probe_func,
    .remove = gpio_remove_func,
    .driver = {
        .name = "gpio_led_driver",
        .of_match_table = gpio_led__match,
    },
};


static void __init gpio_led_init(void)
{
    int reg;
    pr_info("Register GPIO LED driver\n");
    reg = platform_driver_register(&gpio_led_driver);
    if (reg)
        pr_err("Failed to register: %d\n", reg);
}

static void __exit gpio_led_exit(void)
{
    pr_info("Unregister GPIO LED driver\n");
    platform_driver_unregister(&gpio_led_driver);
}

module_init(gpio_led_init);
module_exit(gpio_led_exit);
MODULE_AUTHOR("Likitha");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GPIO blink led using  Platform Driver ");
