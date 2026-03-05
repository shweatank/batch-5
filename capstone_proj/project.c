#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/sched.h>

#define DRIVER_NAME "led_gpio_driver"
#define DEVICE_NAME "led_gpio"
#define CLASS_NAME  "led"

// GPIO Pins
#define GPIO_LED   (22+512)
#define GPIO_INT   (17+512)

static int major;
static struct class*  led_class  = NULL;
static struct device* led_device = NULL;

static int irq_number;
static int interrupt_flag = 0;

DECLARE_WAIT_QUEUE_HEAD(gpio_wait_queue);

/* ---------- WAIT FUNCTION ---------- */
/*
 * Wait for GPIO interrupt event.
 * Returns 0 on success
 */
static int wait_for_gpio_event(void)
{
    int ret;

    ret = wait_event_interruptible(
            gpio_wait_queue,
            interrupt_flag == 1
          );

    if (ret)   // Interrupted by signal
        return ret;

    interrupt_flag = 0;  // Clear event after wakeup

    return 0;
}

/* ---------- Interrupt Handler ---------- */
static irqreturn_t gpio_irq_handler(int irq, void *dev_id)
{
    pr_info("GPIO17 Interrupt Triggered!\n");

    interrupt_flag = 1;

    /* Wake up processes waiting for GPIO event */
    wake_up_interruptible(&gpio_wait_queue);

    return IRQ_HANDLED;
}

/* ---------- File Operations ---------- */

static int led_open(struct inode *inodep, struct file *filep)
{
    pr_info("LED: Device opened\n");
    return 0;
}

static int led_release(struct inode *inodep, struct file *filep)
{
    pr_info("LED: Device closed\n");
    return 0;
}

/* Write = LED Control */
static ssize_t led_write(struct file *filep,
                         const char *buffer,
                         size_t len,
                         loff_t *offset)
{
    char msg[2] = {0};

    if (len > 1)
        len = 1;

    if (copy_from_user(msg, buffer, len))
        return -EFAULT;

    if (msg[0] == '1') {
        gpio_set_value(GPIO_LED, 1);
        pr_info("LED: ON\n");
    }
    else if (msg[0] == '0') {
        gpio_set_value(GPIO_LED, 0);
        pr_info("LED: OFF\n");
    }

    return len;
}

/* Read = Wait for Interrupt Event */
static ssize_t led_read(struct file *filep,
                        char *buffer,
                        size_t len,
                        loff_t *offset)
{
    char msg[] = "GPIO_EVENT\n";
    int ret;

    /* Wait for GPIO interrupt event */
    ret = wait_for_gpio_event();
    if (ret)
        return ret;

    if (copy_to_user(buffer, msg, sizeof(msg)))
        return -EFAULT;

    return sizeof(msg);
}

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = led_open,
    .write   = led_write,
    .read    = led_read,
    .release = led_release,
};

/* ---------- Module Init ---------- */
static int __init led_init(void)
{
    pr_info("LED: Initializing driver...\n");

    gpio_request(GPIO_LED, DRIVER_NAME);
    gpio_direction_output(GPIO_LED, 0);

    gpio_request(GPIO_INT, "gpio_interrupt");
    gpio_direction_input(GPIO_INT);

    irq_number = gpio_to_irq(GPIO_INT);
    pr_info("GPIO mapped to IRQ %d\n", irq_number);

    if (request_irq(irq_number,
                    gpio_irq_handler,
                    IRQF_TRIGGER_RISING,
                    "gpio_irq_handler",
                    NULL)) {
        pr_err("Failed to request IRQ\n");
        return -EBUSY;
    }

    major = register_chrdev(0, DEVICE_NAME, &fops);

    led_class = class_create(CLASS_NAME);
    led_device = device_create(led_class, NULL,
                               MKDEV(major, 0),
                               NULL,
                               DEVICE_NAME);

    pr_info("LED Driver Loaded: /dev/%s\n", DEVICE_NAME);
    return 0;
}

/* ---------- Module Exit ---------- */
static void __exit led_exit(void)
{
    free_irq(irq_number, NULL);

    gpio_set_value(GPIO_LED, 0);
    gpio_free(GPIO_LED);
    gpio_free(GPIO_INT);

    device_destroy(led_class, MKDEV(major, 0));
    class_destroy(led_class);
    unregister_chrdev(major, DEVICE_NAME);

    pr_info("LED Driver Unloaded\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sneha");
MODULE_DESCRIPTION("GPIO LED + Interrupt Driver with Custom Wait Function");
