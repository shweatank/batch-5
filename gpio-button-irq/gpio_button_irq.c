// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/atomic.h>
#include <linux/device.h>
#include <linux/jiffies.h>

#define DRIVER_NAME "gpio_button_irq"

struct gpio_button_data {
	int gpio;
	int irq;
	atomic64_t press_count;
	u32 debounce_ms;
	unsigned long last_irq_jiffies;
};

static irqreturn_t gpio_button_isr(int irq, void *dev_id)
{
	struct gpio_button_data *data = dev_id;
	unsigned long now = jiffies;
	unsigned long debounce_jiffies;
	s64 count;

	/*
	 * Optional software debounce:
	 * ignore edges that arrive too quickly after the previous one.
	 */
	if (data->debounce_ms) {
		debounce_jiffies = msecs_to_jiffies(data->debounce_ms);
		if (time_before(now, data->last_irq_jiffies + debounce_jiffies))
			return IRQ_HANDLED;
	}

	data->last_irq_jiffies = now;
	count = atomic64_inc_return(&data->press_count);
	pr_info("%s: button interrupt handled, count=%lld\n", DRIVER_NAME, count);

	return IRQ_HANDLED;
}

static ssize_t press_count_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct gpio_button_data *data = dev_get_drvdata(dev);

	return scnprintf(buf, PAGE_SIZE, "%lld\n",
			 (long long)atomic64_read(&data->press_count));
}

static DEVICE_ATTR_RO(press_count);

static int gpio_button_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct gpio_button_data *data;
	unsigned long irq_flags = IRQF_TRIGGER_FALLING;
	int ret;

	if (!np)
		return -ENODEV;

	data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	atomic64_set(&data->press_count, 0);
	data->debounce_ms = 30;
	data->last_irq_jiffies = 0;

	data->gpio = of_get_named_gpio(np, "button-gpios", 0);
	if (!gpio_is_valid(data->gpio)) {
		dev_err(dev, "invalid button-gpios from device tree: %d\n", data->gpio);
		return -EINVAL;
	}

	ret = gpio_request(data->gpio, "gpio_button_input");
	if (ret) {
		dev_err(dev, "gpio_request failed for GPIO %d: %d\n", data->gpio, ret);
		return ret;
	}

	ret = gpio_direction_input(data->gpio);
	if (ret) {
		dev_err(dev, "gpio_direction_input failed: %d\n", ret);
		goto err_gpio_free;
	}

	data->irq = gpio_to_irq(data->gpio);
	if (data->irq < 0) {
		dev_err(dev, "gpio_to_irq failed: %d\n", data->irq);
		ret = data->irq;
		goto err_gpio_free;
	}

	if (of_property_read_bool(np, "irq-both-edges"))
		irq_flags = IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING;

	/* Optional DT property: debounce-ms = <30>; */
	of_property_read_u32(np, "debounce-ms", &data->debounce_ms);

	ret = request_irq(data->irq, gpio_button_isr, irq_flags, DRIVER_NAME, data);
	if (ret) {
		dev_err(dev, "request_irq failed for IRQ %d: %d\n", data->irq, ret);
		goto err_gpio_free;
	}

	platform_set_drvdata(pdev, data);
	ret = device_create_file(dev, &dev_attr_press_count);
	if (ret) {
		dev_err(dev, "failed to create press_count sysfs file: %d\n", ret);
		goto err_free_irq;
	}

	dev_info(dev, "probe successful: GPIO=%d IRQ=%d debounce=%u ms\n",
		 data->gpio, data->irq, data->debounce_ms);
	return 0;

err_free_irq:
	free_irq(data->irq, data);
err_gpio_free:
	gpio_free(data->gpio);
	return ret;
}

static int gpio_button_remove(struct platform_device *pdev)
{
	struct gpio_button_data *data = platform_get_drvdata(pdev);

	device_remove_file(&pdev->dev, &dev_attr_press_count);
	free_irq(data->irq, data);
	gpio_free(data->gpio);
	dev_info(&pdev->dev, "driver removed, resources cleaned up\n");

	return 0;
}

static const struct of_device_id gpio_button_of_match[] = {
	{ .compatible = "example,gpio-button-irq" },
	{ }
};
MODULE_DEVICE_TABLE(of, gpio_button_of_match);

static struct platform_driver gpio_button_driver = {
	.probe = gpio_button_probe,
	.remove = gpio_button_remove,
	.driver = {
		.name = DRIVER_NAME,
		.of_match_table = gpio_button_of_match,
	},
};

module_platform_driver(gpio_button_driver);

MODULE_AUTHOR("Cursor Agent");
MODULE_DESCRIPTION("GPIO interrupt-based button Linux kernel driver");
MODULE_LICENSE("GPL");
