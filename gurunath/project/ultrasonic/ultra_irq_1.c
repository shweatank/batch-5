#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/ktime.h>
#include <linux/delay.h>

#define TRIG_GPIO 5
#define ECHO_GPIO 6
#define PROC_NAME "hcsr04_distance"

static int irq_number;
static ktime_t echo_start, echo_end;
static unsigned long distance_cm = 0;
static struct proc_dir_entry *proc_file;

/* Interrupt handler */
static irqreturn_t echo_irq_handler(int irq, void *dev_id)
{
    if (gpio_get_value(ECHO_GPIO)) {
        echo_start = ktime_get();
    } else {
        echo_end = ktime_get();

        s64 duration = ktime_to_us(ktime_sub(echo_end, echo_start));

        if (duration > 0)
            distance_cm = duration / 58;   // Correct HC-SR04 formula
    }

    return IRQ_HANDLED;
}

/* Proc read function */
static ssize_t proc_read(struct file *file, char __user *buf,
                         size_t count, loff_t *pos)
{
    char output[64];
    int len;

    if (*pos > 0)
        return 0;

    /* Send trigger pulse */
    gpio_set_value(TRIG_GPIO, 0);
    udelay(2);
    gpio_set_value(TRIG_GPIO, 1);
    udelay(10);
    gpio_set_value(TRIG_GPIO, 0);

    /* Wait for echo measurement */
    msleep(60);

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

    /* Request TRIG */
    ret = gpio_request(TRIG_GPIO, "trig");
    if (ret) {
        printk(KERN_ERR "Failed to request TRIG GPIO\n");
        return ret;
    }
    gpio_direction_output(TRIG_GPIO, 0);

    /* Request ECHO */
    ret = gpio_request(ECHO_GPIO, "echo");
    if (ret) {
        printk(KERN_ERR "Failed to request ECHO GPIO\n");
        gpio_free(TRIG_GPIO);
        return ret;
    }
    gpio_direction_input(ECHO_GPIO);

    /* Convert GPIO to IRQ */
    irq_number = gpio_to_irq(ECHO_GPIO);
    if (irq_number < 0) {
        printk(KERN_ERR "Failed to get IRQ number\n");
        gpio_free(TRIG_GPIO);
        gpio_free(ECHO_GPIO);
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
        gpio_free(TRIG_GPIO);
        gpio_free(ECHO_GPIO);
        return ret;
    }

    /* Create /proc entry */
    proc_file = proc_create(PROC_NAME, 0666, NULL, &proc_fops);
    if (!proc_file) {
        free_irq(irq_number, NULL);
        gpio_free(TRIG_GPIO);
        gpio_free(ECHO_GPIO);
        return -ENOMEM;
    }

    printk(KERN_INFO "HC-SR04 Driver Loaded Successfully\n");
    return 0;
}

static void __exit hcsr04_exit(void)
{
    proc_remove(proc_file);
    free_irq(irq_number, NULL);
    gpio_free(TRIG_GPIO);
    gpio_free(ECHO_GPIO);

    printk(KERN_INFO "HC-SR04 Driver Unloaded\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("HC-SR04 Ultrasonic Sensor Driver");
module_init(hcsr04_init);
module_exit(hcsr04_exit);
