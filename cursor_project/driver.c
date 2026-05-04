// SPDX-License-Identifier: GPL-2.0
/*
 * Example platform driver with DT matching, MMIO, optional IRQ, optional GPIO,
 * and sysfs control.
 */

#include <linux/bitops.h>
#include <linux/device.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/sizes.h>
#include <linux/slab.h>
#include <linux/sysfs.h>

#define DRV_NAME "demo_platform_device"

/* Fake register map for demonstration. */
#define DEMO_REG_CONTROL         0x00
#define DEMO_REG_STATUS          0x04
#define DEMO_REG_IRQ_STATUS      0x08

#define DEMO_CTRL_ENABLE         BIT(0)
#define DEMO_STATUS_INITIALIZED  BIT(0)
#define DEMO_IRQ_PENDING         BIT(0)

static bool selftest;
module_param(selftest, bool, 0644);
MODULE_PARM_DESC(selftest, "Register a software test platform device");

struct demo_drvdata {
	struct device *dev;
	void __iomem *base;
	int irq;
	u32 irq_count;
	bool use_shadow_regs;
	u32 *shadow_regs;
	struct gpio_desc *enable_gpiod;
};

static u32 demo_reg_read(struct demo_drvdata *ddata, u32 reg)
{
	if (ddata->use_shadow_regs)
		return ddata->shadow_regs[reg / sizeof(u32)];

	return readl(ddata->base + reg);
}

static void demo_reg_write(struct demo_drvdata *ddata, u32 reg, u32 val)
{
	if (ddata->use_shadow_regs) {
		ddata->shadow_regs[reg / sizeof(u32)] = val;
		return;
	}

	writel(val, ddata->base + reg);
}

static void demo_hw_set_enable(struct demo_drvdata *ddata, bool enable)
{
	u32 val;

	val = demo_reg_read(ddata, DEMO_REG_CONTROL);
	if (enable)
		val |= DEMO_CTRL_ENABLE;
	else
		val &= ~DEMO_CTRL_ENABLE;
	demo_reg_write(ddata, DEMO_REG_CONTROL, val);

	if (ddata->enable_gpiod)
		gpiod_set_value_cansleep(ddata->enable_gpiod, enable);
}

static ssize_t enable_show(struct device *dev,
			   struct device_attribute *attr, char *buf)
{
	struct demo_drvdata *ddata = dev_get_drvdata(dev);
	u32 val = demo_reg_read(ddata, DEMO_REG_CONTROL);

	return sysfs_emit(buf, "%u\n", !!(val & DEMO_CTRL_ENABLE));
}

static ssize_t enable_store(struct device *dev,
			    struct device_attribute *attr,
			    const char *buf, size_t count)
{
	struct demo_drvdata *ddata = dev_get_drvdata(dev);
	bool enable;
	int ret;

	ret = kstrtobool(buf, &enable);
	if (ret)
		return ret;

	demo_hw_set_enable(ddata, enable);
	dev_info(dev, "sysfs set enable=%u\n", enable);

	return count;
}

static DEVICE_ATTR_RW(enable);

static irqreturn_t demo_irq_handler(int irq, void *data)
{
	struct demo_drvdata *ddata = data;
	u32 irq_stat;

	irq_stat = demo_reg_read(ddata, DEMO_REG_IRQ_STATUS);
	if (!(irq_stat & DEMO_IRQ_PENDING))
		return IRQ_NONE;

	/* Acknowledge/clear fake interrupt. */
	demo_reg_write(ddata, DEMO_REG_IRQ_STATUS, DEMO_IRQ_PENDING);

	ddata->irq_count++;
	dev_info(ddata->dev, "interrupt handled, count=%u\n", ddata->irq_count);

	return IRQ_HANDLED;
}

static int demo_probe(struct platform_device *pdev)
{
	struct demo_drvdata *ddata;
	struct resource *res;
	int ret;

	dev_info(&pdev->dev, "probe started\n");

	ddata = devm_kzalloc(&pdev->dev, sizeof(*ddata), GFP_KERNEL);
	if (!ddata)
		return -ENOMEM;

	ddata->dev = &pdev->dev;
	platform_set_drvdata(pdev, ddata);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (res) {
		ddata->base = devm_ioremap_resource(&pdev->dev, res);
		if (IS_ERR(ddata->base))
			return PTR_ERR(ddata->base);
	} else if (selftest) {
		/*
		 * Self-test fallback for non-DT desktop environments.
		 * This allows validating probe/remove/sysfs flows.
		 */
		ddata->shadow_regs = devm_kzalloc(&pdev->dev, SZ_4K, GFP_KERNEL);
		if (!ddata->shadow_regs)
			return -ENOMEM;
		ddata->use_shadow_regs = true;
		dev_warn(&pdev->dev, "selftest mode: using shadow registers\n");
	} else {
		dev_err(&pdev->dev, "missing MMIO resource\n");
		return -EINVAL;
	}

	ddata->enable_gpiod = devm_gpiod_get_optional(&pdev->dev, "enable",
						      GPIOD_OUT_LOW);
	if (IS_ERR(ddata->enable_gpiod)) {
		ret = PTR_ERR(ddata->enable_gpiod);
		dev_err(&pdev->dev, "failed to get optional enable GPIO: %d\n", ret);
		return ret;
	}

	/* Simulate hardware init sequence. */
	demo_reg_write(ddata, DEMO_REG_STATUS, DEMO_STATUS_INITIALIZED);
	demo_hw_set_enable(ddata, true);

	ret = device_create_file(&pdev->dev, &dev_attr_enable);
	if (ret) {
		dev_err(&pdev->dev, "failed to create sysfs file: %d\n", ret);
		return ret;
	}

	ddata->irq = platform_get_irq_optional(pdev, 0);
	if (ddata->irq < 0 && ddata->irq != -ENXIO) {
		dev_err(&pdev->dev, "failed to get IRQ: %d\n", ddata->irq);
		ret = ddata->irq;
		goto err_remove_sysfs;
	}

	if (ddata->irq > 0) {
		ret = devm_request_irq(&pdev->dev, ddata->irq, demo_irq_handler,
				       0, DRV_NAME, ddata);
		if (ret) {
			dev_err(&pdev->dev, "request_irq failed: %d\n", ret);
			goto err_remove_sysfs;
		}
		dev_info(&pdev->dev, "IRQ %d registered\n", ddata->irq);
	} else {
		dev_info(&pdev->dev, "no IRQ configured, running in polling mode\n");
	}

	dev_info(&pdev->dev, "probe completed successfully\n");
	return 0;

err_remove_sysfs:
	device_remove_file(&pdev->dev, &dev_attr_enable);
	return ret;
}

static int demo_remove(struct platform_device *pdev)
{
	struct demo_drvdata *ddata = platform_get_drvdata(pdev);

	demo_hw_set_enable(ddata, false);
	device_remove_file(&pdev->dev, &dev_attr_enable);
	dev_info(&pdev->dev, "removed\n");

	return 0;
}

static const struct of_device_id demo_of_match[] = {
	{ .compatible = "demo,my-platform-device" },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, demo_of_match);

static struct platform_driver demo_driver = {
	.probe = demo_probe,
	.remove = demo_remove,
	.driver = {
		.name = DRV_NAME,
		.of_match_table = demo_of_match,
	},
};

static struct platform_device *demo_test_pdev;

static int __init demo_init(void)
{
	int ret;

	ret = platform_driver_register(&demo_driver);
	if (ret)
		return ret;

	if (selftest) {
		demo_test_pdev = platform_device_register_simple(DRV_NAME,
								 PLATFORM_DEVID_NONE,
								 NULL, 0);
		if (IS_ERR(demo_test_pdev)) {
			ret = PTR_ERR(demo_test_pdev);
			platform_driver_unregister(&demo_driver);
			return ret;
		}
		pr_info(DRV_NAME ": selftest platform device registered\n");
	}

	return 0;
}

static void __exit demo_exit(void)
{
	if (selftest && demo_test_pdev)
		platform_device_unregister(demo_test_pdev);

	platform_driver_unregister(&demo_driver);
}

module_init(demo_init);
module_exit(demo_exit);

MODULE_AUTHOR("Kernel Driver Example");
MODULE_DESCRIPTION("Production-style example Linux platform driver");
MODULE_LICENSE("GPL");
