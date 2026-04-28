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
#include <linux/jiffies.h>

#define DEVICE_NAME "hcsr04"
#define TRIG 23   // BCM 23
#define ECHO 24   // BCM 24

static dev_t devno;
static struct cdev cdev;
static struct task_struct *thread;
static unsigned int distance_mm;

static int measure_thread(void *arg)
{
    while (!kthread_should_stop()) {

        ktime_t start, end;
        s64 duration;
        unsigned long timeout;

        /* Trigger pulse 10us */
        gpio_set_value(TRIG, 0);
        udelay(2);
        gpio_set_value(TRIG, 1);
        udelay(10);
        gpio_set_value(TRIG, 0);

        /* Wait for ECHO HIGH */
        timeout = jiffies + msecs_to_jiffies(100);
        while (!gpio_get_value(ECHO)) {
            if (kthread_should_stop() ||
                time_after(jiffies, timeout))
                goto sleep_label;
            cpu_relax();
        }

        start = ktime_get();

        /* Wait for ECHO LOW */
        timeout = jiffies + msecs_to_jiffies(100);
        while (gpio_get_value(ECHO)) {
            if (kthread_should_stop() ||
                time_after(jiffies, timeout))
                goto sleep_label;
            cpu_relax();
        }

        end = ktime_get();

        duration = ktime_to_ns(ktime_sub(end, start));

        /* distance in mm */
        distance_mm = (unsigned int)
            ((duration * 343000ULL) / (2ULL * 1000000000ULL));

sleep_label:
        msleep(60);   // HC-SR04 requires ≥60ms delay
    }

    return 0;
}

static ssize_t hcsr04_read(struct file *file,
                           char __user *buf,
                           size_t len,
                           loff_t *off)
{
    char kbuf[32];
    int bytes;

    if (*off > 0)
        return 0;

    bytes = snprintf(kbuf, sizeof(kbuf),
                     "%u.%u cm\n",
                     distance_mm / 10,
                     distance_mm % 10);

    if (copy_to_user(buf, kbuf, bytes))
        return -EFAULT;

    *off += bytes;
    return bytes;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read  = hcsr04_read,
};

static int __init hcsr04_init(void)
{
    int ret;

    printk(KERN_INFO "HC-SR04 driver init\n");

    if (!gpio_is_valid(TRIG) ||
        !gpio_is_valid(ECHO))
        return -EINVAL;

    ret = gpio_request(TRIG, "TRIG");
    if (ret) return ret;

    ret = gpio_request(ECHO, "ECHO");
    if (ret) {
        gpio_free(TRIG);
        return ret;
    }

    gpio_direction_output(TRIG, 0);
    gpio_direction_input(ECHO);

    ret = alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME);
    if (ret)
        goto gpio_err;

    cdev_init(&cdev, &fops);
    ret = cdev_add(&cdev, devno, 1);
    if (ret)
        goto chrdev_err;

    thread = kthread_run(measure_thread, NULL,
                         "hcsr04_thread");
    if (IS_ERR(thread)) {
        ret = PTR_ERR(thread);
        goto cdev_err;
    }

    printk(KERN_INFO
           "Create device:\n"
           "sudo mknod /dev/hcsr04 c %d 0\n"
           "sudo chmod 666 /dev/hcsr04\n",
           MAJOR(devno));

    return 0;

cdev_err:
    cdev_del(&cdev);
chrdev_err:
    unregister_chrdev_region(devno, 1);
gpio_err:
    gpio_free(TRIG);
    gpio_free(ECHO);
    return ret;
}

static void __exit hcsr04_exit(void)
{
    if (thread)
        kthread_stop(thread);

    cdev_del(&cdev);
    unregister_chrdev_region(devno, 1);

    gpio_free(TRIG);
    gpio_free(ECHO);

    printk(KERN_INFO "HC-SR04 driver unloaded\n");
}

module_init(hcsr04_init);
module_exit(hcsr04_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kernel Module");
MODULE_DESCRIPTION("HC-SR04 polling driver for Raspberry Pi 4");

