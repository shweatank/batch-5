#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/uaccess.h>
#include <linux/ktime.h>
#include <linux/delay.h>

#define PROC_NAME "hcsr04_direction_led"

static struct gpio_desc *trig;
static struct gpio_desc *echo;
static struct gpio_desc *led_near;   // GPIO17
static struct gpio_desc *led_far;    // GPIO27

static int irq_number;
static ktime_t echo_start, echo_end;
static unsigned long distance_cm = 0;

/* Speed variables */
static long prev_distance_cm = 0;
static ktime_t prev_time;
static long speed_cm_per_sec = 0;

static struct proc_dir_entry *proc_file;

/* ---------------- Interrupt Handler ---------------- */
static irqreturn_t echo_irq_handler(int irq, void *dev_id)
{
    if (gpiod_get_value(echo))
        echo_start = ktime_get();
    else {
        echo_end = ktime_get();
        s64 duration_us = ktime_to_us(ktime_sub(echo_end, echo_start));
        if (duration_us > 0)
            distance_cm = duration_us / 58;   // Distance in cm
    }

    return IRQ_HANDLED;
}

/* ---------------- PROC READ FUNCTION ---------------- */
static ssize_t proc_read(struct file *file, char __user *buf,
                         size_t count, loff_t *pos)
{
    char output[128];
    int len, i;
    ktime_t current_time;
    s64 time_diff_ms;
    long distance_diff;
    long display_speed = 0;
    unsigned int blink_delay = 150;

    if (*pos > 0)
        return 0;

    /* Trigger ultrasonic sensor */
    gpiod_set_value(trig, 0);
    udelay(2);
    gpiod_set_value(trig, 1);
    udelay(10);
    gpiod_set_value(trig, 0);

    msleep(60);

    /* ---- Speed Calculation ---- */
    current_time = ktime_get();
    time_diff_ms = ktime_to_ms(ktime_sub(current_time, prev_time));

    if (time_diff_ms > 0) {
        distance_diff = distance_cm - prev_distance_cm;
        speed_cm_per_sec = (distance_diff * 1000) / time_diff_ms;
    }

    prev_distance_cm = distance_cm;
    prev_time = current_time;

    /* Absolute value for display */
    if (speed_cm_per_sec < 0)
        display_speed = -speed_cm_per_sec;
    else
        display_speed = speed_cm_per_sec;

    /* Turn off both LEDs */
    gpiod_set_value(led_near, 0);
    gpiod_set_value(led_far, 0);

    /* ---- Direction Based LED Blink ---- */

    if (speed_cm_per_sec < 0) {
        /* Object moving closer */
        for (i = 0; i < 5; i++) {
            gpiod_set_value(led_near, 1);
            msleep(blink_delay);
            gpiod_set_value(led_near, 0);
            msleep(blink_delay);
        }
    }
    else if (speed_cm_per_sec > 0) {
        /* Object moving away */
        for (i = 0; i < 5; i++) {
            gpiod_set_value(led_far, 1);
            msleep(blink_delay);
            gpiod_set_value(led_far, 0);
            msleep(blink_delay);
        }
    }

    /* Output to user */
    len = sprintf(output,
                  "Distance: %lu cm\nSpeed: %ld cm/s\n",
                  distance_cm,
                  display_speed);

    if (copy_to_user(buf, output, len))
        return -EFAULT;

    *pos = len;
    return len;
}

static const struct proc_ops proc_fops = {
    .proc_read = proc_read,
};

/* ---------------- Device Tree Match ---------------- */
static const struct of_device_id hcsr04_of_match[] = {
    { .compatible = "mycompany,hcsr04", },
    {},
};
MODULE_DEVICE_TABLE(of, hcsr04_of_match);

/* ---------------- Probe ---------------- */
static int hcsr04_probe(struct platform_device *pdev)
{
    int ret;

    trig = devm_gpiod_get(&pdev->dev, "trig", GPIOD_OUT_LOW);
    echo = devm_gpiod_get(&pdev->dev, "echo", GPIOD_IN);
    led_near = devm_gpiod_get(&pdev->dev, "led-near", GPIOD_OUT_LOW);
    led_far  = devm_gpiod_get(&pdev->dev, "led-far", GPIOD_OUT_LOW);

    if (IS_ERR(trig) || IS_ERR(echo) ||
        IS_ERR(led_near) || IS_ERR(led_far)) {
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

    prev_time = ktime_get();

    dev_info(&pdev->dev, "HC-SR04 Direction Detection Driver Loaded\n");
    return 0;
}

/* ---------------- Remove ---------------- */
static void hcsr04_remove(struct platform_device *pdev)
{
    proc_remove(proc_file);
    free_irq(irq_number, NULL);

    gpiod_set_value(led_near, 0);
    gpiod_set_value(led_far, 0);

    dev_info(&pdev->dev, "HC-SR04 Driver Unloaded\n");
}

static struct platform_driver hcsr04_driver = {
    .probe  = hcsr04_probe,
    .remove = hcsr04_remove,
    .driver = {
        .name = "hcsr04_direction_led",
        .of_match_table = hcsr04_of_match,
    },
};

module_platform_driver(hcsr04_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("HC-SR04 Direction Detection with Two LEDs (Positive Speed Output)");
