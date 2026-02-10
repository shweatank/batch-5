#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h>
#include <linux/timer.h>

struct gpio_led_data {
    struct gpio_desc *gpiod;
    struct timer_list blink_timer;
    bool state;
};

static void led_blink_timer(struct timer_list *t)
{
    struct gpio_led_data *led =
        from_timer(led, t, blink_timer);

    led->state = !led->state;
    gpiod_set_value(led->gpiod, led->state);

    mod_timer(&led->blink_timer, jiffies + msecs_to_jiffies(1000));
}

static int gpio_led_probe(struct platform_device *pdev)
{
    struct gpio_led_data *led;

    led = devm_kzalloc(&pdev->dev, sizeof(*led), GFP_KERNEL);
    if (!led)
        return -ENOMEM;

    led->gpiod = devm_gpiod_get(&pdev->dev, "led", GPIOD_OUT_LOW);
    if (IS_ERR(led->gpiod)) {
        dev_err(&pdev->dev, "Failed to get GPIO\n");
        return PTR_ERR(led->gpiod);
    }

    timer_setup(&led->blink_timer, led_blink_timer, 0);
    mod_timer(&led->blink_timer, jiffies + msecs_to_jiffies(1000));

    platform_set_drvdata(pdev, led);

    dev_info(&pdev->dev, "GPIO LED driver probed\n");
    return 0;
}

static int gpio_led_remove(struct platform_device *pdev)
{
    struct gpio_led_data *led = platform_get_drvdata(pdev);

    del_timer_sync(&led->blink_timer);
    gpiod_set_value(led->gpiod, 0);

    dev_info(&pdev->dev, "GPIO LED driver removed\n");
    return 0;
}

static const struct of_device_id gpio_led_of_match[] = {
    { .compatible = "techdhaba,gpio-led" },
    { }
};
MODULE_DEVICE_TABLE(of, gpio_led_of_match);

static struct platform_driver gpio_led_driver = {
    .probe  = gpio_led_probe,
    .remove = gpio_led_remove,
    .driver = {
        .name = "gpio-led-dt",
        .of_match_table = gpio_led_of_match,
    },
};

module_platform_driver(gpio_led_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("GPIO LED Driver using DTS for Raspberry Pi 4B");
