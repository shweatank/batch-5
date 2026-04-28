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
static struct gpio_desc *led_close;   // GPIO17
static struct gpio_desc *led_far;     // GPIO27

static int irq_number;
static ktime_t echo_start, echo_end;
static unsigned long distance_cm = 0;
static struct proc_dir_entry *proc_file;

/* Interrupt handler */
static irqreturn_t echo_irq_handler(int irq, void *dev_id)
{
    if (gpiod_get_value(echo)) {
        echo_start = ktime_get();
    } else {
        echo_end = ktime_get();
        s64 duration_us = ktime_to_us(ktime_sub(echo_end, echo_start));
        if (duration_us > 0)
            distance_cm = duration_us / 58;
    }

    return IRQ_HANDLED;
}

/* /proc read */
static ssize_t proc_read(struct file *file, char __user *buf,
                         size_t count, loff_t *pos)
{
    char output[128];
    int len;

    static unsigned long prev_distance = 0;
    static ktime_t prev_time;
    static int first_read = 1;

    long speed_cms = 0;
    long delta_distance = 0;

    if (*pos > 0)
        return 0;

    /* Trigger pulse */
    gpiod_set_value(trig, 0);
    udelay(2);
    gpiod_set_value(trig, 1);
    udelay(10);
    gpiod_set_value(trig, 0);

    msleep(60);

    if (!first_read) {
        ktime_t current_time = ktime_get();
        s64 delta_time_us =
            ktime_to_us(ktime_sub(current_time, prev_time));

        delta_distance = (long)distance_cm - (long)prev_distance;

        if (delta_time_us > 0)
            speed_cms = (delta_distance * 1000000L) / delta_time_us;

        prev_time = current_time;
    } else {
        prev_time = ktime_get();
        first_read = 0;
    }

    /* -------- LED CONTROL -------- */
/* -------- Frequency Based LED Control -------- */

if (!first_read) {

    int i;
    int delay;
    long change = delta_distance;

    if (change < 0)   /* approaching */
        change = -change;

    if (change == 0) {
        gpiod_set_value(led_close, 0);
        gpiod_set_value(led_far, 0);
    }
    else {

        /* Convert movement speed to blink frequency */
        if (change > 20)
            delay = 20;      // extremely fast
        else if (change > 10)
            delay = 40;      // very fast
        else if (change > 5)
            delay = 70;      // medium
        else
            delay = 120;     // slow

        if (delta_distance < 0) {
            /* Object coming closer → blink GPIO17 */
            for (i = 0; i < 12; i++) {
                gpiod_set_value(led_close, 1);
                msleep(delay);
                gpiod_set_value(led_close, 0);
                msleep(delay);
            }
            gpiod_set_value(led_far, 0);
        }
        else {
            /* Object moving away → blink GPIO27 */
            for (i = 0; i < 12; i++) {
                gpiod_set_value(led_far, 1);
                msleep(delay);
                gpiod_set_value(led_far, 0);
                msleep(delay);
            }
            gpiod_set_value(led_close, 0);
        }
    }
}

    /* -------- FAST LED CONTROL -------- */

if (!first_read) {

    int i;
    int delay = 60;   // default fast blink

    /* Make it faster if very close */
    if (distance_cm < 10)
        delay = 30;   // extremely fast
    else if (distance_cm < 20)
        delay = 50;   // very fast

    if (delta_distance < 0) {
        /* Object coming closer → blink GPIO17 fast */
        for (i = 0; i < 10; i++) {
            gpiod_set_value(led_close, 1);
            msleep(delay);
            gpiod_set_value(led_close, 0);
            msleep(delay);
        }
        gpiod_set_value(led_far, 0);
    }
    else if (delta_distance > 0) {
        /* Object moving away → blink GPIO27 fast */
        for (i = 0; i < 10; i++) {
            gpiod_set_value(led_far, 1);
            msleep(delay);
            gpiod_set_value(led_far, 0);
            msleep(delay);
        }
        gpiod_set_value(led_close, 0);
    }
    else {
        gpiod_set_value(led_close, 0);
        gpiod_set_value(led_far, 0);
    }
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

/* Probe */
static int hcsr04_probe(struct platform_device *pdev)
{
    int ret;

    trig = devm_gpiod_get(&pdev->dev, "trig", GPIOD_OUT_LOW);
    echo = devm_gpiod_get(&pdev->dev, "echo", GPIOD_IN);

    /* NEW LED GPIOs */
    led_close = devm_gpiod_get(&pdev->dev, "ledclose", GPIOD_OUT_LOW);
    led_far   = devm_gpiod_get(&pdev->dev, "ledfar", GPIOD_OUT_LOW);

    if (IS_ERR(trig) || IS_ERR(echo) ||
        IS_ERR(led_close) || IS_ERR(led_far)) {
        dev_err(&pdev->dev, "Failed to get GPIOs\n");
        return -ENODEV;
    }

    irq_number = gpiod_to_irq(echo);
    if (irq_number < 0)
        return irq_number;

    ret = request_irq(irq_number,
                      echo_irq_handler,
                      IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                      "hcsr04_irq",
                      NULL);
    if (ret)
        return ret;

    proc_file = proc_create(PROC_NAME, 0666, NULL, &proc_fops);
    if (!proc_file) {
        free_irq(irq_number, NULL);
        return -ENOMEM;
    }

    dev_info(&pdev->dev, "HC-SR04 + LED Driver Loaded\n");
    return 0;
}

static void hcsr04_remove(struct platform_device *pdev)
{
    proc_remove(proc_file);
    free_irq(irq_number, NULL);
}

/* DT match */
static const struct of_device_id hcsr04_of_match[] = {
    { .compatible = "mycompany,hcsr04", },
    {},
};
MODULE_DEVICE_TABLE(of, hcsr04_of_match);

static struct platform_driver hcsr04_driver = {
    .probe  = hcsr04_probe,
    .remove = hcsr04_remove,
    .driver = {
        .name = "hcsr04_distance",
        .of_match_table = hcsr04_of_match,
    },
};

module_platform_driver(hcsr04_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("HC-SR04 Driver with LED Direction Indicator");
