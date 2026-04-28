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
            distance_cm = duration / 58;  // HC-SR04 formula
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

    msleep(60);  // wait for echo

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

    /* Get GPIO descriptors from DT overlay */
  /*  trig = gpiod_get(NULL, "trig", GPIOD_OUT_LOW);
    echo = gpiod_get(NULL, "echo", GPIOD_IN);*/

    trig = gpiod_get(NULL, "HC-SR04 TRIG", GPIOD_OUT_LOW);
    echo = gpiod_get(NULL, "HC-SR04 ECHO", GPIOD_IN);
    if (IS_ERR(trig) || IS_ERR(echo)) {
        printk(KERN_ERR "Failed to get GPIO descriptors\n");
        return -ENODEV;
    }

    /* Map ECHO GPIO to IRQ */
    irq_number = gpiod_to_irq(echo);
    if (irq_number < 0) {
        printk(KERN_ERR "Failed to get IRQ number\n");
        gpiod_put(trig);
        gpiod_put(echo);
        return irq_number;
    }

    /* Request IRQ for rising/falling edges */
    ret = request_irq(irq_number,
                      echo_irq_handler,
                      IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                      "hcsr04_irq",
                      NULL);
    if (ret) {
        printk(KERN_ERR "Failed to request IRQ\n");
        gpiod_put(trig);
        gpiod_put(echo);
        return ret;
    }

    /* Create /proc entry */
    proc_file = proc_create(PROC_NAME, 0666, NULL, &proc_fops);
    if (!proc_file) {
        free_irq(irq_number, NULL);
        gpiod_put(trig);
        gpiod_put(echo);
        return -ENOMEM;
    }

    printk(KERN_INFO "HC-SR04 gpiod Kernel Driver Loaded\n");
    return 0;
}

static void __exit hcsr04_exit(void)
{
    proc_remove(proc_file);
    free_irq(irq_number, NULL);
    gpiod_put(trig);
    gpiod_put(echo);
    printk(KERN_INFO "HC-SR04 Driver Unloaded\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("HC-SR04 Ultrasonic Sensor Kernel Driver");
module_init(hcsr04_init);
module_exit(hcsr04_exit);
