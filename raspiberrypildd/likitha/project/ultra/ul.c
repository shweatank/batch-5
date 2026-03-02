#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/ktime.h>

#define TRIG 17
#define ECHO 27

static int __init ultrasonic_init(void)
{
    ktime_t start, end;
    s64 duration;
    unsigned long distance;

    printk(KERN_INFO "Ultrasonic driver loaded\n");

    if (gpio_request(TRIG, "TRIG")) {
        printk(KERN_ERR "Cannot request TRIG\n");
        return -1;
    }

    if (gpio_request(ECHO, "ECHO")) {
        printk(KERN_ERR "Cannot request ECHO\n");
        gpio_free(TRIG);
        return -1;
    }

    gpio_direction_output(TRIG, 0);
    gpio_direction_input(ECHO);

    /* Send 10us trigger pulse */
    gpio_set_value(TRIG, 0);
    udelay(2);
    gpio_set_value(TRIG, 1);
    udelay(10);
    gpio_set_value(TRIG, 0);

    /* Wait for echo to go HIGH */
    while (gpio_get_value(ECHO) == 0);

    start = ktime_get();

    /* Wait for echo to go LOW */
    while (gpio_get_value(ECHO) == 1);

    end = ktime_get();

    duration = ktime_to_us(ktime_sub(end, start));

    /* Distance calculation (cm) */
    distance = duration / 58;

    printk(KERN_INFO "Distance: %lu cm\n", distance);

    return 0;
}

static void __exit ultrasonic_exit(void)
{
    gpio_free(TRIG);
    gpio_free(ECHO);
    printk(KERN_INFO "Ultrasonic driver removed\n");
}

module_init(ultrasonic_init);
module_exit(ultrasonic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Team");
MODULE_DESCRIPTION("Ultrasonic Sensor Kernel Module");
