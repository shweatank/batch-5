#include <linux/module.h>           // Core header for modules
#include <linux/platform_device.h>  // For platform_driver and platform_device
#include <linux/of.h>               // For Device Tree matching
#include <linux/gpio/consumer.h>    // For GPIO consumer API
#include <linux/workqueue.h>        // For delayed_work

#define BLINK_DELAY_MS 1000  // Delay between LED toggles (1 second)


// Context structure to store LED state and work
struct blink_ctx {
    struct gpio_desc *led;          // GPIO descriptor for the LED
    struct delayed_work blink_work; // Delayed work for blinking
    bool led_on;                    // Current LED state (true=ON, false=OFF)
};


// Function executed by workqueue to toggle LED
static void blink_work_fn(struct work_struct *work)
{
    // Get pointer to our context from work structure
    struct blink_ctx *ctx = container_of(to_delayed_work(work), struct blink_ctx, blink_work);

    // Toggle LED state
    ctx->led_on = !ctx->led_on;
    gpiod_set_value(ctx->led, ctx->led_on); // Set GPIO value

    // Schedule the next toggle after 1 second
    schedule_delayed_work(&ctx->blink_work, msecs_to_jiffies(BLINK_DELAY_MS));
}


// Probe function: called when Device Tree node matches this driver
static int blink_probe(struct platform_device *pdev)
{
    struct blink_ctx *ctx;

    // Allocate memory for our context
    ctx = devm_kzalloc(&pdev->dev, sizeof(*ctx), GFP_KERNEL);
    if (!ctx)
        return -ENOMEM;

    // Get LED GPIO from Device Tree
    // NULL = default GPIO property name
    // GPIOD_OUT_LOW = configure as output, initial state LOW
    ctx->led = devm_gpiod_get(&pdev->dev, NULL, GPIOD_OUT_LOW);
    if (IS_ERR(ctx->led))
        return PTR_ERR(ctx->led);

    // Initialize the delayed work
    INIT_DELAYED_WORK(&ctx->blink_work, blink_work_fn);

    // Schedule first blink after BLINK_DELAY_MS
    schedule_delayed_work(&ctx->blink_work, msecs_to_jiffies(BLINK_DELAY_MS));

    // Print info to kernel log
    dev_info(&pdev->dev, "Blink LED driver loaded\n");

    // Save context in platform device for use in remove()
    platform_set_drvdata(pdev, ctx);

    return 0;
}


// Remove function: called when driver/module is removed
static void blink_remove(struct platform_device *pdev)
{
    struct blink_ctx *ctx = platform_get_drvdata(pdev);

    // Cancel any scheduled work safely
    cancel_delayed_work_sync(&ctx->blink_work);

    // Turn off LED
    gpiod_set_value(ctx->led, 0);

    dev_info(&pdev->dev, "Blink LED driver removed\n");
    
}


// Device Tree match table
// Matches "techdhaba,blink-led" nodes in Device Tree
static const struct of_device_id blink_of_match[] = {
    { .compatible = "techdhaba,blink-led" },
    { }
};
MODULE_DEVICE_TABLE(of, blink_of_match);


// Platform driver structure
static struct platform_driver blink_driver = {
    .probe  = blink_probe,
    .remove = blink_remove,
    .driver = {
        .name = "blink_led",
        .of_match_table = blink_of_match,
    },
};

// Register driver with kernel
module_platform_driver(blink_driver);


// Module info
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Simple LED blink driver (1 second interval)");

