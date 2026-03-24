// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/regulator/consumer.h>
#include <linux/slab.h>

struct mydev_drvdata {
	void __iomem *base;
	int irq;
	struct clk *clk;
	struct regulator *vdd;
	struct device *dev;
	spinlock_t lock;
};

static irqreturn_t mydev_irq_handler(int irq, void *dev_id)
{
	struct mydev_drvdata *drvdata = dev_id;
	unsigned long flags;

	spin_lock_irqsave(&drvdata->lock, flags);
	// TODO: Handle device interrupt
	spin_unlock_irqrestore(&drvdata->lock, flags);
	return IRQ_HANDLED;
}

static int mydev_probe(struct platform_device *pdev)
{
	struct mydev_drvdata *drvdata;
	struct resource *res;
	int ret;

	drvdata = devm_kzalloc(&pdev->dev, sizeof(*drvdata), GFP_KERNEL);
	if (!drvdata)
		return -ENOMEM;
	drvdata->dev = &pdev->dev;
	spin_lock_init(&drvdata->lock);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	drvdata->base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(drvdata->base))
		return PTR_ERR(drvdata->base);

	drvdata->clk = devm_clk_get_optional(&pdev->dev, NULL);
	if (IS_ERR(drvdata->clk))
		return PTR_ERR(drvdata->clk);
	if (drvdata->clk) {
		ret = clk_prepare_enable(drvdata->clk);
		if (ret)
			return ret;
	}

	drvdata->vdd = devm_regulator_get_optional(&pdev->dev, "vdd");
	if (IS_ERR(drvdata->vdd) && PTR_ERR(drvdata->vdd) != -ENODEV)
		goto err_clk;
	if (!IS_ERR_OR_NULL(drvdata->vdd)) {
		ret = regulator_enable(drvdata->vdd);
		if (ret)
			goto err_clk;
	}

	drvdata->irq = platform_get_irq(pdev, 0);
	if (drvdata->irq < 0) {
		ret = drvdata->irq;
		goto err_reg;
	}
	ret = devm_request_irq(&pdev->dev, drvdata->irq, mydev_irq_handler, 0,
			pdev->name, drvdata);
	if (ret)
		goto err_reg;

	platform_set_drvdata(pdev, drvdata);
	dev_info(&pdev->dev, "mydev probed\n");
	return 0;

err_reg:
	if (!IS_ERR_OR_NULL(drvdata->vdd))
		regulator_disable(drvdata->vdd);
err_clk:
	if (drvdata->clk)
		clk_disable_unprepare(drvdata->clk);
	return ret;
}

static int mydev_remove(struct platform_device *pdev)
{
	struct mydev_drvdata *drvdata = platform_get_drvdata(pdev);

	if (!IS_ERR_OR_NULL(drvdata->vdd))
		regulator_disable(drvdata->vdd);
	if (drvdata->clk)
		clk_disable_unprepare(drvdata->clk);
	return 0;
}

static const struct of_device_id mydev_of_match[] = {
	{ .compatible = "myvendor,mydev", },
	{ }
};
MODULE_DEVICE_TABLE(of, mydev_of_match);

static struct platform_driver mydev_driver = {
	.probe = mydev_probe,
	.remove = mydev_remove,
	.driver = {
		.name = "mydev",
		.of_match_table = mydev_of_match,
	},
};

module_platform_driver(mydev_driver);

MODULE_AUTHOR("Your Name <your@email.com>");
MODULE_DESCRIPTION("Example production-quality platform driver");
MODULE_LICENSE("GPL v2");
