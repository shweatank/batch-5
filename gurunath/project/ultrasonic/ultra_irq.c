#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/ktime.h>

#define DEVICE_NAME "hcsr04_poll"

/* GPIO pins (BCM numbering) */
#define TRIG 17 // output
#define ECHO 27 // input

static dev_t devno;
static struct cdev cdev;
static struct task_struct *thread;
static unsigned int distance_mm;

/* Kernel thread: trigger HC-SR04 and measure distance */
static int hcsr04_thread_fn(void *data)
{
    while (!kthread_should_stop()) {
        ktime_t start, end;
        s64 duration_ns;
        unsigned long timeout;

        /* Trigger 10µs pulse */
        gpio_set_value(TRIG, 0);
        udelay(2);
        gpio_set_value(TRIG, 1);
        udelay(10);
        gpio_set_value(TRIG, 0);

        /* Wait for ECHO HIGH with 100ms timeout */
        timeout = jiffies + msecs_to_jiffies(100);
        while (!gpio_get_value(ECHO)) {
            if (kthread_should_stop() || time_after(jiffies, timeout))
                goto skip_measure;
            udelay(1);
        }
        start = ktime_get();

        /* Wait for ECHO LOW with 100ms timeout */
        timeout = jiffies + msecs_to_jiffies(100);
        while (gpio_get_value(ECHO)) {
            if (kthread_should_stop() || time_after(jiffies, timeout))
                goto skip_measure;
            udelay(1);
        }
        end = ktime_get();

        /* Calculate distance in mm */
        duration_ns = ktime_to_ns(ktime_sub(end, start));
        distance_mm = (unsigned int)((duration_ns * 343000ULL) / (2ULL * 1000000000ULL));

    skip_measure:
        /* Wait 60ms before next measurement (HC-SR04 requirement) */
        msleep(60);
    }

    return 0;
}

/* read() implementation */
static ssize_t hcsr04_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
    char kbuf[32];
    int bytes;

    if (*offset > 0)
        return 0;

    bytes = snprintf(kbuf, sizeof(kbuf), "%u.%u cm\n", distance_mm/10, distance_mm%10);
    if (len < bytes)
        return -EINVAL;

    if (copy_to_user(buf, kbuf, bytes))
        return -EFAULT;

    *offset += bytes;
    return bytes;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read  = hcsr04_read,
};

/* Module init */
static int __init hcsr04_init(void)
{
    int ret;

    printk(KERN_INFO "HC-SR04 polling driver init\n");

    /* Allocate char device */
    ret = alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME);
    if (ret)
        return ret;

    cdev_init(&cdev, &fops);
    ret = cdev_add(&cdev, devno, 1);
    if (ret)
        goto err_region;

    /* Request GPIOs */
    if (!gpio_is_valid(TRIG) || !gpio_is_valid(ECHO)) {
        printk(KERN_ERR "Invalid GPIOs\n");
        ret = -ENODEV;
        goto err_cdev;
    }

    gpio_request(TRIG, "TRIG");
    gpio_request(ECHO, "ECHO");
    gpio_direction_output(TRIG, 0);
    gpio_direction_input(ECHO);

    /* Start kernel thread */
    thread = kthread_run(hcsr04_thread_fn, NULL, "hcsr04_thread");
    if (IS_ERR(thread)) {
        ret = PTR_ERR(thread);
        goto err_gpio;
    }

    printk(KERN_INFO "Driver loaded. Create device node:\n");
    printk(KERN_INFO "sudo mknod /dev/hcsr04 c %d 0\n", MAJOR(devno));
    printk(KERN_INFO "sudo chmod 666 /dev/hcsr04\n");

    return 0;

err_gpio:
    gpio_free(TRIG);
    gpio_free(ECHO);
err_cdev:
    cdev_del(&cdev);
err_region:
    unregister_chrdev_region(devno, 1);
    return ret;
}

/* Module exit */
static void __exit hcsr04_exit(void)
{
    if (thread)
        kthread_stop(thread);

    gpio_free(TRIG);
    gpio_free(ECHO);

    cdev_del(&cdev);
    unregister_chrdev_region(devno, 1);

    printk(KERN_INFO "HC-SR04 polling driver unloaded\n");
}

module_init(hcsr04_init);
module_exit(hcsr04_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("HC-SR04 kernel driver using polling (works on Pi 4)");

