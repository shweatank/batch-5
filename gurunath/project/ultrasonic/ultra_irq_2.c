#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
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
        echo_start = ktime_get();
    } else {
        echo_end = ktime_get();
        s64 duration = ktime_to_us(ktime_sub(echo_end, echo_start));
        if (duration > 0)
            distance_cm = duration / 58;  // Correct HC-SR04 formula
    }
    return IRQ_HANDLED;
}

/* /proc read function */
static ssize_t proc_read(struct file *file, char __user *buf,
                         size_t count, loff_t *pos)
{
    char output[64];
    int len;

    if (*pos > 0)
        return 0;

    /* Trigger pulse */
    gpiod_set_value(trig, 0);
    udelay(2);
    gpiod_set_value(trig, 1);
    udelay(10);
    gpiod_set_value(trig, 0);

    msleep(60);  // wait for measurement

    len = sprintf(output, "Distance: %lu cm\n", distance_cm);
    if (copy_to_user(buf, output, len))
        return -EFAULT;

    *pos = len;
    return len;
}

static const struct proc_ops proc_fops = {
    .proc_read = proc_read,
};

static int __init hcsr04_init(void)
{
    int ret;

    /* Get GPIO descriptors (BCM 5 = TRIG, BCM 6 = ECHO) */
    trig = gpio_to_desc(5);
    echo = gpio_to_desc(6);

    if (!trig || !echo) {
        printk(KERN_ERR "Failed to get GPIO descriptors\n");
        return -ENODEV;
    }

    gpiod_direction_output(trig, 0);
    gpiod_direction_input(echo);

    /* Map ECHO GPIO to IRQ */
    irq_number = gpiod_to_irq(echo);
    if (irq_number < 0) {
        printk(KERN_ERR "Failed to get IRQ number\n");
        return irq_number;
    }

    /* Request IRQ */
    ret = request_irq(irq_number,
                      echo_irq_handler,
                      IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                      "hcsr04_irq",
                      NULL);
    if (ret) {
        printk(KERN_ERR "Failed to request IRQ\n");
        return ret;
    }

    /* Create /proc entry */
    proc_file = proc_create(PROC_NAME, 0666, NULL, &proc_fops);
    if (!proc_file) {
        free_irq(irq_number, NULL);
        return -ENOMEM;
    }

    printk(KERN_INFO "HC-SR04 gpiod Driver Loaded\n");
    return 0;
}

static void __exit hcsr04_exit(void)
{
    proc_remove(proc_file);
    free_irq(irq_number, NULL);
    printk(KERN_INFO "HC-SR04 Driver Unloaded\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("HC-SR04 Ultrasonic Sensor Driver (gpiod-based)");
module_init(hcsr04_init);
module_exit(hcsr04_exit);
