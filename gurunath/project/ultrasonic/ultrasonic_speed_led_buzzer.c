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
static struct gpio_desc *led_close;  // GPIO17
static struct gpio_desc *led_far;    // GPIO27
static struct gpio_desc *buzzer;     // GPIO22

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
            distance_cm = duration_us / 58;  // Convert to cm
    }

    return IRQ_HANDLED;
}

/* /proc read function: distance + LED + buzzer feedback */
static ssize_t proc_read(struct file *file, char __user *buf,
                         size_t count, loff_t *pos)
{
    char output[128];
    int len;

    if (*pos > 0)
        return 0;

    /* Trigger pulse */
    gpiod_set_value(trig, 0);
    udelay(2);
    gpiod_set_value(trig, 1);
    udelay(10);
    gpiod_set_value(trig, 0);

    /* Wait for echo to be measured by interrupt */
    msleep(60);

    /* LED + Buzzer Threshold Logic */
    if (distance_cm <= 10) {
        /* Very close → LED17 + buzzer very fast blink */
        for (int i = 0; i < 10; i++) {
            gpiod_set_value(led_close, 1);
            gpiod_set_value(buzzer, 1);
            msleep(50);
            gpiod_set_value(led_close, 0);
            gpiod_set_value(buzzer, 0);
            msleep(50);
        }
        gpiod_set_value(led_far, 0);
    }
    else if (distance_cm <= 20) {
        /* Close → LED17 + buzzer medium blink */
        for (int i = 0; i < 8; i++) {
            gpiod_set_value(led_close, 1);
            gpiod_set_value(buzzer, 1);
            msleep(100);
            gpiod_set_value(led_close, 0);
            gpiod_set_value(buzzer, 0);
            msleep(100);
        }
        gpiod_set_value(led_far, 0);
    }
    else if (distance_cm <= 40) {
        /* Moving away → LED27 blink only */
        for (int i = 0; i < 6; i++) {
            gpiod_set_value(led_far, 1);
            msleep(150);
            gpiod_set_value(led_far, 0);
            msleep(150);
        }
        gpiod_set_value(led_close, 0);
        gpiod_set_value(buzzer, 0);
    }
    else {
        /* Safe → turn off all LEDs and buzzer */
        gpiod_set_value(led_close, 0);
        gpiod_set_value(led_far, 0);
        gpiod_set_value(buzzer, 0);
    }

    /* Return distance to /proc */
    len = sprintf(output, "Dis: %lu cm\n", distance_cm);

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
    led_close = devm_gpiod_get(&pdev->dev, "led-close", GPIOD_OUT_LOW);
    led_far   = devm_gpiod_get(&pdev->dev, "led-far", GPIOD_OUT_LOW);
    buzzer    = devm_gpiod_get(&pdev->dev, "buzzer", GPIOD_OUT_LOW);

    if (IS_ERR(trig) || IS_ERR(echo) ||
        IS_ERR(led_close) || IS_ERR(led_far) || IS_ERR(buzzer)) {
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

    /* Turn off LEDs and buzzer */
    gpiod_set_value(led_close, 0);
    gpiod_set_value(led_far, 0);
    gpiod_set_value(buzzer, 0);

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
MODULE_DESCRIPTION("HC-SR04 Ultrasonic Sensor Driver with LEDs and Buzzer");
