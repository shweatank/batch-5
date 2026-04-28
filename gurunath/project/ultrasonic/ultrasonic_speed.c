#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/uaccess.h>
#include <linux/ktime.h>
#include <linux/delay.h>

#define PROC_NAME "hcsr04_distance"

static struct gpio_desc *trig;
static struct gpio_desc *echo;
static int irq_number;
static ktime_t echo_start, echo_end;
static unsigned long distance_cm = 0;
static struct proc_dir_entry *proc_file;

/* Interrupt handler for ECHO */
static irqreturn_t echo_irq_handler(int irq, void *dev_id)
{
    if (gpiod_get_value(echo)) {
        /* Rising edge: start timing */
        echo_start = ktime_get();
    } else {
        /* Falling edge: stop timing */
        echo_end = ktime_get();

        s64 duration_us = ktime_to_us(ktime_sub(echo_end, echo_start));
        if (duration_us > 0)
            distance_cm = duration_us / 58;  // HC-SR04 formula in cm
    }

    return IRQ_HANDLED;
}

/* /proc read function with speed calculation */
static ssize_t proc_read(struct file *file, char __user *buf,
                         size_t count, loff_t *pos)
{
    char output[128];
    int len;

    /* Static variables to track previous measurement */
    static unsigned long prev_distance = 0;
    static ktime_t prev_time;
    static int first_read = 1;
    long speed_cms = 0;

    if (*pos > 0)
        return 0;

    /* Send trigger pulse */
    gpiod_set_value(trig, 0);
    udelay(2);
    gpiod_set_value(trig, 1);
    udelay(10);
    gpiod_set_value(trig, 0);

    /* Wait for echo measurement */
    msleep(60);

    /* Calculate speed based on previous reading */
    if (!first_read) {
        ktime_t current_time = ktime_get();
        s64 delta_time_us = ktime_to_us(ktime_sub(current_time, prev_time));

        if (delta_time_us > 0) {
            long delta_distance = (long)distance_cm - (long)prev_distance;
            speed_cms = (delta_distance * 1000000L) / delta_time_us;
        }

        prev_time = current_time;
    } else {
        prev_time = ktime_get();
        first_read = 0;
    }

    prev_distance = distance_cm;

    len = sprintf(output,
                  "D: %lu cm\nSp: %ld cm/s\n",
                  distance_cm, speed_cms);

    if (copy_to_user(buf, output, len))
        return -EFAULT;

    *pos = len;
    return len;
}

static const struct proc_ops proc_fops = {
    .proc_read = proc_read,
};

/* Device Tree match table */
static const struct of_device_id hcsr04_of_match[] = {
    { .compatible = "mycompany,hcsr04", },
    {},
};
MODULE_DEVICE_TABLE(of, hcsr04_of_match);

/* Probe function */
static int hcsr04_probe(struct platform_device *pdev)
{
    int ret;

    /* Get GPIOs from device tree */
    trig = devm_gpiod_get(&pdev->dev, "trig", GPIOD_OUT_LOW);
    echo = devm_gpiod_get(&pdev->dev, "echo", GPIOD_IN);
    if (IS_ERR(trig) || IS_ERR(echo)) {
        dev_err(&pdev->dev, "Failed to get GPIOs\n");
        return -ENODEV;
    }

    /* Map ECHO GPIO to IRQ */
    irq_number = gpiod_to_irq(echo);
    if (irq_number < 0) {
        dev_err(&pdev->dev, "Failed to get IRQ number\n");
        return irq_number;
    }

    ret = request_irq(irq_number,
                      echo_irq_handler,
                      IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                      "hcsr04_irq",
                      NULL);
    if (ret) {
        dev_err(&pdev->dev, "Failed to request IRQ\n");
        return ret;
    }

    /* Create /proc entry */
    proc_file = proc_create(PROC_NAME, 0666, NULL, &proc_fops);
    if (!proc_file) {
        free_irq(irq_number, NULL);
        return -ENOMEM;
    }

    dev_info(&pdev->dev, "HC-SR04 Kernel Driver Loaded\n");
    return 0;
}

/* Remove function */
static void hcsr04_remove(struct platform_device *pdev)
{
    proc_remove(proc_file);
    free_irq(irq_number, NULL);
    dev_info(&pdev->dev, "HC-SR04 Driver Unloaded\n");
}

/* Platform driver definition */
static struct platform_driver hcsr04_driver = {
    .probe  = hcsr04_probe,
    .remove = hcsr04_remove,
    .driver = {
        .name = "hcsr04_distance",
        .owner = THIS_MODULE,
        .of_match_table = hcsr04_of_match,
    },
};

module_platform_driver(hcsr04_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("HC-SR04 Ultrasonic Sensor Kernel Driver with Distance and Speed");
