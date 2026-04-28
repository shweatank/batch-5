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

/* BCM GPIO numbers (change if needed) */
#define TRIG 12
#define ECHO 13

static int major=0;
static struct cdev hcsr04_cdev;
static struct task_struct *thread;
static unsigned int distance_mm = 0;
static bool running = true;

/* ---------------- Measurement Thread ---------------- */

static int hcsr04_thread(void *arg)
{
    while (!kthread_should_stop()) {

        ktime_t start, end;
        s64 duration_ns;
        unsigned long timeout;

        /* Trigger 10us pulse */
        gpio_set_value(TRIG, 0);
        udelay(2);
        gpio_set_value(TRIG, 1);
        udelay(10);
        gpio_set_value(TRIG, 0);

        /* Wait for ECHO high (with timeout 50ms) */
        timeout = jiffies + msecs_to_jiffies(50);
        while (!gpio_get_value(ECHO)) {
            if (kthread_should_stop() || time_after(jiffies, timeout)) {
                distance_mm = 0;
                goto sleep_label;
            }
            cpu_relax();
        }

        start = ktime_get();

        /* Wait for ECHO low (with timeout 50ms) */
        timeout = jiffies + msecs_to_jiffies(50);
        while (gpio_get_value(ECHO)) {
            if (kthread_should_stop() || time_after(jiffies, timeout)) {
                distance_mm = 0;
                goto sleep_label;
            }
            cpu_relax();
        }

        end = ktime_get();

        duration_ns = ktime_to_ns(ktime_sub(end, start));

        /* Distance in mm */
        distance_mm = (unsigned int)
            ((duration_ns * 343000ULL) / (2ULL * 1000000000ULL));

sleep_label:
        msleep(100);
    }

    return 0;
}

/* ---------------- Read Function ---------------- */

static ssize_t hcsr04_read(struct file *file,
                           char __user *buf,
                           size_t len,
                           loff_t *offset)
{
    char kbuf[32];
    int bytes;

    if (*offset > 0)
        return 0;

    bytes = snprintf(kbuf, sizeof(kbuf),
                     "%u.%u cm\n",
                     distance_mm / 10,
                     distance_mm % 10);

    if (copy_to_user(buf, kbuf, bytes))
        return -EFAULT;

    *offset += bytes;
    return bytes;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read  = hcsr04_read,
};

/* ---------------- Module Init ---------------- */

static int __init hcsr04_init(void)
{
    int ret;

    printk(KERN_INFO "HC-SR04 kernel driver init\n");

    /* Allocate char device */
    ret = alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME);
    if (ret)
        return ret;

    cdev_init(&hcsr04_cdev, &fops);
    ret = cdev_add(&hcsr04_cdev, devno, 1);
    if (ret)
        goto err_unregister;

    /* Request GPIOs */
    if (!gpio_is_valid(TRIG) || !gpio_is_valid(ECHO)) {
        ret = -ENODEV;
        goto err_cdev;
    }

    ret = gpio_request(TRIG, "hcsr04_trig");
    if (ret)
        goto err_cdev;

    ret = gpio_request(ECHO, "hcsr04_echo");
    if (ret)
        goto err_trig;

    gpio_direction_output(TRIG, 0);
    gpio_direction_input(ECHO);

    /* Start kernel thread */
    thread = kthread_run(hcsr04_thread, NULL, "hcsr04_thread");
    if (IS_ERR(thread)) {
        ret = PTR_ERR(thread);
        goto err_echo;
    }

    printk(KERN_INFO "Create device node:\n");
    printk(KERN_INFO "sudo mknod /dev/hcsr04 c %d 0\n", MAJOR(devno));
    printk(KERN_INFO "sudo chmod 666 /dev/hcsr04\n");

    return 0;

err_echo:
    gpio_free(ECHO);
err_trig:
    gpio_free(TRIG);
err_cdev:
    cdev_del(&hcsr04_cdev);
err_unregister:
    unregister_chrdev_region(devno, 1);
    return ret;
}

/* ---------------- Module Exit ---------------- */

static void __exit hcsr04_exit(void)
{
    if (thread)
        kthread_stop(thread);

    gpio_free(TRIG);
    gpio_free(ECHO);

    cdev_del(&hcsr04_cdev);
    unregister_chrdev_region(devno, 1);

    printk(KERN_INFO "HC-SR04 driver unloaded\n");
}

module_init(hcsr04_init);
module_exit(hcsr04_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("HC-SR04 polling kernel driver (stable)");

