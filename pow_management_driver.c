#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/clk.h>
#include <linux/regulator/consumer.h>
#include <linux/pm_runtime.h>

/* -------------------------------------------------
 * Private Driver Data
 * ------------------------------------------------- */
struct pm_example_dev {
    struct clk *clk;
    struct regulator *vdd;
};

/* -------------------------------------------------
 * Suspend / Resume (System Sleep)
 * ------------------------------------------------- */
static int pm_example_suspend(struct device *dev)
{
    struct pm_example_dev *priv = dev_get_drvdata(dev);

    dev_info(dev, "x86 system suspend\n");

    if (priv->clk)
        clk_disable_unprepare(priv->clk);

    if (priv->vdd)
        regulator_disable(priv->vdd);

    return 0;
}

static int pm_example_resume(struct device *dev)
{
    struct pm_example_dev *priv = dev_get_drvdata(dev);
    int ret;

    dev_info(dev, "x86 system resume\n");

    if (priv->vdd) {
        ret = regulator_enable(priv->vdd);
        if (ret)
            return ret;
    }

    if (priv->clk) {
        ret = clk_prepare_enable(priv->clk);
        if (ret)
            return ret;
    }

    return 0;
}

/* -------------------------------------------------
 * Runtime PM
 * ------------------------------------------------- */
static int pm_example_runtime_suspend(struct device *dev)
{
    struct pm_example_dev *priv = dev_get_drvdata(dev);

    dev_info(dev, "x86 runtime suspend\n");

    if (priv->clk)
        clk_disable_unprepare(priv->clk);

    if (priv->vdd)
        regulator_disable(priv->vdd);

    return 0;
}

static int pm_example_runtime_resume(struct device *dev)
{
    struct pm_example_dev *priv = dev_get_drvdata(dev);
    int ret;

    dev_info(dev, "x86 runtime resume\n");

    if (priv->vdd) {
        ret = regulator_enable(priv->vdd);
        if (ret)
            return ret;
    }

    if (priv->clk) {
        ret = clk_prepare_enable(priv->clk);
        if (ret)
            return ret;
    }

    return 0;
}

/* -------------------------------------------------
 * PM Ops (Kernel 6.8 Correct)
 * ------------------------------------------------- */
static const struct dev_pm_ops pm_example_pm_ops = {
    SET_SYSTEM_SLEEP_PM_OPS(pm_example_suspend,
                            pm_example_resume)
    SET_RUNTIME_PM_OPS(pm_example_runtime_suspend,
                       pm_example_runtime_resume,
                       NULL)
};

/* -------------------------------------------------
 * Probe / Remove
 * ------------------------------------------------- */
static int pm_example_probe(struct platform_device *pdev)
{
    struct pm_example_dev *priv;
    int ret;

    dev_info(&pdev->dev, "x86 PM driver probe\n");

    priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
    if (!priv)
        return -ENOMEM;

    priv->clk = devm_clk_get_optional(&pdev->dev, NULL);
    if (IS_ERR(priv->clk))
        return PTR_ERR(priv->clk);

    priv->vdd = devm_regulator_get_optional(&pdev->dev, "vdd");
    if (IS_ERR(priv->vdd))
        return PTR_ERR(priv->vdd);

    if (priv->vdd) {
        ret = regulator_enable(priv->vdd);
        if (ret)
            return ret;
    }

    if (priv->clk) {
        ret = clk_prepare_enable(priv->clk);
        if (ret)
            return ret;
    }

    platform_set_drvdata(pdev, priv);

    pm_runtime_enable(&pdev->dev);
    pm_runtime_set_active(&pdev->dev);

    return 0;
}

static int pm_example_remove(struct platform_device *pdev)
{
    struct pm_example_dev *priv = platform_get_drvdata(pdev);

    dev_info(&pdev->dev, "x86 PM driver remove\n");

    pm_runtime_disable(&pdev->dev);

    if (priv->clk)
        clk_disable_unprepare(priv->clk);

    if (priv->vdd)
        regulator_disable(priv->vdd);

    return 0;
}

/* -------------------------------------------------
 * Platform Driver
 * ------------------------------------------------- */
static struct platform_driver pm_example_driver = {
    .probe  = pm_example_probe,
    .remove = pm_example_remove,
    .driver = {
        .name = "pm_example_x86",
        .pm   = &pm_example_pm_ops,
    },
};

/* -------------------------------------------------
 * Explicit Platform Device (CRITICAL FOR x86 TESTING)
 * ------------------------------------------------- */
static struct platform_device *pm_example_pdev;

/* -------------------------------------------------
 * Module Init / Exit
 * ------------------------------------------------- */
static int __init pm_example_init(void)
{
    int ret;

    ret = platform_driver_register(&pm_example_driver);
    if (ret)
        return ret;

    pm_example_pdev =
        platform_device_register_simple("pm_example_x86", -1, NULL, 0);

    if (IS_ERR(pm_example_pdev)) {
        platform_driver_unregister(&pm_example_driver);
        return PTR_ERR(pm_example_pdev);
    }

    return 0;
}

static void __exit pm_example_exit(void)
{
    platform_device_unregister(pm_example_pdev);
    platform_driver_unregister(&pm_example_driver);
}

module_init(pm_example_init);
module_exit(pm_example_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prashant / TechDhaba");
MODULE_DESCRIPTION("x86 Power Management Driver (Kernel 6.8, Verified)");

/*
echo mem  | sudo tee /sys/power/state
mem

//went to slight sleep 
mirafra@mirafra:~/raspberrypi$ echo deep | sudo tee /sys/power/mem_sleep
deep
tee: /sys/power/mem_sleep: Invalid argument
//that means deep sleep is disableed in modern systems by manufactureer
mirafra@mirafra:~/raspberrypi$ cat /sys/power/mem_sleep
[s2idle]
//only slight sleep is abled..
*/
