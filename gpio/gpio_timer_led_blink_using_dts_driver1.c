#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/gpio/consumer.h>
#include <linux/workqueue.h>

#define BLINK_DELAY_MS 1000  // 1 second

struct blink_ctx {
    struct gpio_desc *led;
    struct delayed_work blink_work;
    bool led_on;
};

static void blink_work_fn(struct work_struct *work)
{
    struct blink_ctx *ctx = container_of(to_delayed_work(work), struct blink_ctx, blink_work);

    ctx->led_on = !ctx->led_on;
    gpiod_set_value(ctx->led, ctx->led_on);

    schedule_delayed_work(&ctx->blink_work, msecs_to_jiffies(BLINK_DELAY_MS));
}

static int blink_probe(struct platform_device *pdev)
{
    struct blink_ctx *ctx;

    ctx = devm_kzalloc(&pdev->dev, sizeof(*ctx), GFP_KERNEL);
    if (!ctx)
        return -ENOMEM;

    ctx->led = devm_gpiod_get(&pdev->dev, NULL, GPIOD_OUT_LOW);
    if (IS_ERR(ctx->led))
        return PTR_ERR(ctx->led);

    INIT_DELAYED_WORK(&ctx->blink_work, blink_work_fn);

    schedule_delayed_work(&ctx->blink_work, msecs_to_jiffies(BLINK_DELAY_MS));

    dev_info(&pdev->dev, "Blink LED driver loaded\n");
    platform_set_drvdata(pdev, ctx);

    return 0;
}

static int blink_remove(struct platform_device *pdev)
{
    struct blink_ctx *ctx = platform_get_drvdata(pdev);

    cancel_delayed_work_sync(&ctx->blink_work);
    gpiod_set_value(ctx->led, 0); // turn off LED
    dev_info(&pdev->dev, "Blink LED driver removed\n");

    return 0;
}

static const struct of_device_id blink_of_match[] = {
    { .compatible = "techdhaba,blink-led" },
    { }
};
MODULE_DEVICE_TABLE(of, blink_of_match);

static struct platform_driver blink_driver = {
    .probe  = blink_probe,
    .remove = blink_remove,
    .driver = {
        .name = "blink_led",
        .of_match_table = blink_of_match,
    },
};

module_platform_driver(blink_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Simple LED blink driver");
