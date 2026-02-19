#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/platform_device.h>

struct ky028_data {
    int gpio;
    int irq;
    struct work_struct work;
};

static void ky028_work_func(struct work_struct *work)
{
    struct ky028_data *data = container_of(work, struct ky028_data, work);
    pr_info("KY-028 Digital Temperature Threshold Triggered on GPIO %d\n", data->gpio);
}

static irqreturn_t ky028_irq_handler(int irq, void *dev_id)
{
    struct ky028_data *data = dev_id;
    pr_info("KY-028 GPIO Interrupt Detected (IRQ %d)\n", irq);
    schedule_work(&data->work);
    return IRQ_HANDLED;
}

static int ky028_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    struct ky028_data *data;
    int ret;

    pr_info("KY-028 Driver Probe\n");

    data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);
    if (!data)
        return -ENOMEM;

    platform_set_drvdata(pdev, data);

    /* Get GPIO from device tree */
    data->gpio = of_get_named_gpio(dev->of_node, "gpios", 0);
    if (!gpio_is_valid(data->gpio)) {
        dev_err(dev, "Invalid GPIO\n");
        return -EINVAL;
    }

    ret = devm_gpio_request_one(dev, data->gpio, GPIOF_IN, "ky028_do");
    if (ret) {
        dev_err(dev, "Failed to request GPIO %d\n", data->gpio);
        return ret;
    }

    /* Initialize workqueue */
    INIT_WORK(&data->work, ky028_work_func);

    /* Map GPIO to IRQ */
    data->irq = gpio_to_irq(data->gpio);
    if (data->irq < 0) {
        dev_err(dev, "Failed to map GPIO %d to IRQ\n", data->gpio);
        return data->irq;
    }

    /* Request IRQ for rising edge (DO goes HIGH) */
    ret = devm_request_irq(dev,
                           data->irq,
                           ky028_irq_handler,
                           IRQF_TRIGGER_RISING,
                           "ky028_irq",
                           data);
    if (ret) {
        dev_err(dev, "Failed to request IRQ %d\n", data->irq);
        return ret;
    }

    pr_info("KY-028 Driver Initialized (GPIO %d, IRQ %d)\n", data->gpio, data->irq);

    return 0;
}

static int ky028_remove(struct platform_device *pdev)
{
    struct ky028_data *data = platform_get_drvdata(pdev);
    cancel_work_sync(&data->work);
    pr_info("KY-028 Driver Removed\n");
    return 0;
}

static const struct of_device_id ky028_of_match[] = {
    { .compatible = "gpio-ky028" },
    { }
};
MODULE_DEVICE_TABLE(of, ky028_of_match);

static struct platform_driver ky028_driver = {
    .probe  = ky028_probe,
    .remove = ky028_remove,
    .driver = {
        .name = "ky028_driver",
        .of_match_table = ky028_of_match,
    },
};

module_platform_driver(ky028_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Team4");
MODULE_DESCRIPTION("KY-028 Temperature Digital Output Driver via GPIO + Workqueue");

