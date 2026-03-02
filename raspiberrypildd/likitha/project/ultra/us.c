
#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/ktime.h>

#define TRIG_PIN 23
#define ECHO_PIN 24

static int trig_pin, echo_pin;

static int __init ultrasonic_init(void) {
    int ret;
    ktime_t start, end;
    s64 pulse_duration;

    // Set up GPIO pins
    ret = gpio_request(TRIG_PIN, "trig_pin");
    if (ret) {
        printk(KERN_ERR "Failed to request TRIG pin\n");
        return ret;
    }
    ret = gpio_request(ECHO_PIN, "echo_pin");
    if (ret) {
        printk(KERN_ERR "Failed to request ECHO pin\n");
        gpio_free(TRIG_PIN);
        return ret;
    }

    gpio_direction_output(TRIG_PIN, 0);
    gpio_direction_input(ECHO_PIN);

    // Send trigger pulse
    gpio_set_value(TRIG_PIN, 1);
    udelay(10);
    gpio_set_value(TRIG_PIN, 0);

    // Wait for echo pulse
    while (gpio_get_value(ECHO_PIN) == 0);
    start = ktime_get();
    while (gpio_get_value(ECHO_PIN) == 1);
    end = ktime_get();

    // Calculate distance
    pulse_duration = ktime_to_ns(ktime_sub(end, start));
    pulse_duration /= 1000; // Convert to microseconds
    long distance = pulse_duration * 17150 / 1000000;
    printk(KERN_INFO "Distance: %ld cm\n", distance);

    gpio_free(TRIG_PIN);
    gpio_free(ECHO_PIN);

    return 0;
}

static void __exit ultrasonic_exit(void) {
    printk(KERN_INFO "Ultrasonic module removed\n");
}

module_init(ultrasonic_init);
module_exit(ultrasonic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Ultrasonic sensor detection module");
