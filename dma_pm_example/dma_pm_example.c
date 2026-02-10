#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/clk.h>
#include <linux/regulator/consumer.h>
#include <linux/pm_runtime.h>
#include <linux/delay.h>

/* -------------------------------------------------
 * Private DMA Device Structure
 * ------------------------------------------------- */
struct dma_pm_dev {
    struct clk *clk;
    struct regulator *vdd;
    bool dma_active;
};

/* -------------------------------------------------
 * Fake DMA helpers (replace with real HW ops)
 * ------------------------------------------------- */
static void dma_hw_stop(struct dma_pm_dev *d)
{
    /* In real driver:
     *  - Mask DMA IRQ
     *  - Clear RUN bit
     *  - Wait for IDLE
     */
    d->dma_active = false;
    pr_info("dma_pm: DMA engine stopped\n");
}

static void dma_hw_start(struct dma_pm_dev *d)
{
    d->dma_active = true;
    pr_info("dma_pm: DMA engine started\n");
}

/* -------------------------------------------------
 * System Suspend / Resume
 * ------------------------------------------------- */
static int dma_pm_suspend(struct device *dev)
{
    struct dma_pm_dev *d = dev_get_drvdata(dev);

    dev_info(dev, "DMA system suspend\n");

    /* 1. Stop DMA safely */
    if (d->dma_active)
        dma_hw_stop(d);

    /* 2. Disable clocks */
    if (d->clk)
        clk_disable_unprepare(d->clk);

    /* 3. Disable regulator */
    if (d->vdd)
        regulator_disable(d->vdd);

    return 0;
}

static int dma_pm_resume(struct device *dev)
{
    struct dma_pm_dev *d = dev_get_drvdata(dev);
    int ret;

    dev_info(dev, "DMA system resume\n");

    /* 1. Power ON */
    if (d->vdd) {
        ret = regulator_enable(d->vdd);
        if (ret)
            return ret;
    }

    /* 2. Enable clock */
    if (d->clk) {
        ret = clk_prepare_enable(d->clk);
        if (ret)
            return ret;
    }

    /* 3. Restart DMA if required */
    dma_hw_start(d);

    return 0;
}

/* -------------------------------------------------
 * Runtime PM
 * ------------------------------------------------- */
static int dma_pm_runtime_suspend(struct device *dev)
{
    struct dma_pm_dev *d = dev_get_drvdata(dev);

    dev_info(dev, "DMA runtime suspend\n");

    if (d->dma_active)
        dma_hw_stop(d);

    if (d->clk)
        clk_disable_unprepare(d->clk);

    if (d->vdd)
        regulator_disable(d->vdd);

    return 0;
}

static int dma_pm_runtime_resume(struct device *dev)
{
    struct dma_pm_dev *d = dev_get_drvdata(dev);
    int ret;

    dev_info(dev, "DMA runtime resume\n");

    if (d->vdd) {
        ret = regulator_enable(d->vdd);
        if (ret)
            return ret;
    }

    if (d->clk) {
        ret = clk_prepare_enable(d->clk);
        if (ret)
            return ret;
    }

    dma_hw_start(d);
    return 0;
}

/* -------------------------------------------------
 * PM Ops (Kernel 6.8)
 * ------------------------------------------------- */
static const struct dev_pm_ops dma_pm_ops = {
    SET_SYSTEM_SLEEP_PM_OPS(dma_pm_suspend,
                            dma_pm_resume)
    SET_RUNTIME_PM_OPS(dma_pm_runtime_suspend,
                       dma_pm_runtime_resume,
                       NULL)
};

/* -------------------------------------------------
 * Probe / Remove
 * ------------------------------------------------- */
static int dma_pm_probe(struct platform_device *pdev)
{
    struct dma_pm_dev *d;
    int ret;

    dev_info(&pdev->dev, "DMA PM driver probe\n");

    d = devm_kzalloc(&pdev->dev, sizeof(*d), GFP_KERNEL);
    if (!d)
        return -ENOMEM;

    d->clk = devm_clk_get_optional(&pdev->dev, NULL);
    if (IS_ERR(d->clk))
        return PTR_ERR(d->clk);

    d->vdd = devm_regulator_get_optional(&pdev->dev, "vdd");
    if (IS_ERR(d->vdd))
        return PTR_ERR(d->vdd);

    if (d->vdd) {
        ret = regulator_enable(d->vdd);
        if (ret)
            return ret;
    }

    if (d->clk) {
        ret = clk_prepare_enable(d->clk);
        if (ret)
            return ret;
    }

    dma_hw_start(d);

    platform_set_drvdata(pdev, d);

    pm_runtime_enable(&pdev->dev);
    pm_runtime_set_active(&pdev->dev);

    return 0;
}

static int dma_pm_remove(struct platform_device *pdev)
{
    struct dma_pm_dev *d = platform_get_drvdata(pdev);

    dev_info(&pdev->dev, "DMA PM driver remove\n");

    pm_runtime_disable(&pdev->dev);

    if (d->dma_active)
        dma_hw_stop(d);

    if (d->clk)
        clk_disable_unprepare(d->clk);

    if (d->vdd)
        regulator_disable(d->vdd);

    return 0;
}

/* -------------------------------------------------
 * Platform Driver
 * ------------------------------------------------- */
static struct platform_driver dma_pm_driver = {
    .probe  = dma_pm_probe,
    .remove = dma_pm_remove,
    .driver = {
        .name = "dma_pm_example_x86",
        .pm   = &dma_pm_ops,
    },
};

/* -------------------------------------------------
 * Explicit Platform Device (x86 requirement)
 * ------------------------------------------------- */
static struct platform_device *dma_pm_pdev;

/* -------------------------------------------------
 * Module Init / Exit
 * ------------------------------------------------- */
static int __init dma_pm_init(void)
{
    int ret;

    ret = platform_driver_register(&dma_pm_driver);
    if (ret)
        return ret;

    dma_pm_pdev =
        platform_device_register_simple("dma_pm_example_x86", -1, NULL, 0);

    if (IS_ERR(dma_pm_pdev)) {
        platform_driver_unregister(&dma_pm_driver);
        return PTR_ERR(dma_pm_pdev);
    }

    return 0;
}

static void __exit dma_pm_exit(void)
{
    platform_device_unregister(dma_pm_pdev);
    platform_driver_unregister(&dma_pm_driver);
}

module_init(dma_pm_init);
module_exit(dma_pm_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhi");
MODULE_DESCRIPTION("x86 DMA Power Management Driver (Kernel 6.8)");
