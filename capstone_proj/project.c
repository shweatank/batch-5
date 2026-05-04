#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/cdev.h>
#include <linux/atomic.h>

#define DRIVER_NAME "led_gpio_driver"
#define DEVICE_NAME "led_gpio"
#define CLASS_NAME  "led"

// GPIO Pins
#define GPIO_LED   (22+512)
#define GPIO_INT   (17+512)

static dev_t dev_num;
static struct class *led_class;
static struct device *led_device;
static struct cdev led_cdev;

static int irq_number;
static atomic_t interrupt_flag = ATOMIC_INIT(0);

DECLARE_WAIT_QUEUE_HEAD(gpio_wait_queue);

/* ---------- WAIT FUNCTION ---------- */
/*
 * Wait for GPIO interrupt event.
 * Returns 0 on success
 */
static int wait_for_gpio_event(void)
{
    int ret;

    ret = wait_event_interruptible(gpio_wait_queue,
                                    atomic_read(&interrupt_flag) != 0);

    if (ret)   // Interrupted by signal
        return ret;

    /* Clear event after wakeup */
    atomic_set(&interrupt_flag, 0);

    return 0;
}

/* ---------- Interrupt Handler ---------- */
static irqreturn_t gpio_irq_handler(int irq, void *dev_id)
{
    (void)dev_id;
    pr_info("GPIO interrupt triggered (irq=%d)\n", irq);
    atomic_set(&interrupt_flag, 1);

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
    char val;
    int ret;

    (void)filep;
    (void)offset;

    if (len < 1)
        return -EINVAL;

    ret = copy_from_user(&val, buffer, 1);
    if (ret != 0)
        return -EFAULT;

    switch (val) {
    case '1':
        gpio_set_value(GPIO_LED, 1);
        pr_info("LED: ON\n");
        break;
    case '0':
        gpio_set_value(GPIO_LED, 0);
        pr_info("LED: OFF\n");
        break;
    default:
        return -EINVAL;
    }

    return 1;
}

/* Read = Wait for Interrupt Event */
static ssize_t led_read(struct file *filep,
                        char *buffer,
                        size_t len,
                        loff_t *offset)
{
    static const char msg[] = "GPIO_EVENT\n";
    const size_t msg_len = sizeof(msg) - 1;
    int ret;

    (void)filep;
    (void)offset;

    if (len < msg_len)
        return -EMSGSIZE;

    /* Wait for GPIO interrupt event */
    ret = wait_for_gpio_event();
    if (ret)
        return ret;

    if (copy_to_user(buffer, msg, msg_len))
        return -EFAULT;

    return (ssize_t)msg_len;
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
    int ret;

    pr_info("LED: Initializing driver...\n");

    ret = gpio_request_one(GPIO_LED, GPIOF_OUT_INIT_LOW, DRIVER_NAME);
    if (ret) {
        pr_err("Failed to request GPIO_LED=%d: %d\n", GPIO_LED, ret);
        return ret;
    }

    ret = gpio_request_one(GPIO_INT, GPIOF_IN, "gpio_interrupt");
    if (ret) {
        pr_err("Failed to request GPIO_INT=%d: %d\n", GPIO_INT, ret);
        gpio_free(GPIO_LED);
        return ret;
    }

    irq_number = gpio_to_irq(GPIO_INT);
    if (irq_number < 0) {
        pr_err("gpio_to_irq(GPIO_INT=%d) failed: %d\n", GPIO_INT, irq_number);
        gpio_free(GPIO_INT);
        gpio_free(GPIO_LED);
        return irq_number;
    }

    pr_info("GPIO mapped to IRQ %d\n", irq_number);

    ret = request_irq(irq_number,
                       gpio_irq_handler,
                       IRQF_TRIGGER_RISING,
                       "gpio_irq_handler",
                       NULL);
    if (ret) {
        pr_err("Failed to request IRQ=%d: %d\n", irq_number, ret);
        gpio_free(GPIO_INT);
        gpio_free(GPIO_LED);
        return ret;
    }

    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_err("alloc_chrdev_region failed: %d\n", ret);
        free_irq(irq_number, NULL);
        gpio_free(GPIO_INT);
        gpio_free(GPIO_LED);
        return ret;
    }

    cdev_init(&led_cdev, &fops);
    led_cdev.owner = THIS_MODULE;
    ret = cdev_add(&led_cdev, dev_num, 1);
    if (ret) {
        pr_err("cdev_add failed: %d\n", ret);
        unregister_chrdev_region(dev_num, 1);
        free_irq(irq_number, NULL);
        gpio_free(GPIO_INT);
        gpio_free(GPIO_LED);
        return ret;
    }

    led_class = class_create(CLASS_NAME);
    if (IS_ERR(led_class)) {
        ret = PTR_ERR(led_class);
        pr_err("class_create failed: %d\n", ret);
        cdev_del(&led_cdev);
        unregister_chrdev_region(dev_num, 1);
        free_irq(irq_number, NULL);
        gpio_free(GPIO_INT);
        gpio_free(GPIO_LED);
        return ret;
    }

    led_device = device_create(led_class, NULL, dev_num, NULL, DEVICE_NAME);
    if (IS_ERR(led_device)) {
        ret = PTR_ERR(led_device);
        pr_err("device_create failed: %d\n", ret);
        class_destroy(led_class);
        cdev_del(&led_cdev);
        unregister_chrdev_region(dev_num, 1);
        free_irq(irq_number, NULL);
        gpio_free(GPIO_INT);
        gpio_free(GPIO_LED);
        return ret;
    }

    pr_info("LED Driver Loaded: /dev/%s\n", DEVICE_NAME);
    return 0;
}

/* ---------- Module Exit ---------- */
static void __exit led_exit(void)
{
    pr_info("LED: Unloading driver...\n");

    free_irq(irq_number, NULL);

    gpio_set_value(GPIO_LED, 0);
    gpio_free(GPIO_LED);
    gpio_free(GPIO_INT);

    device_destroy(led_class, dev_num);
    class_destroy(led_class);
    cdev_del(&led_cdev);
    unregister_chrdev_region(dev_num, 1);

    pr_info("LED Driver Unloaded\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sneha");
MODULE_DESCRIPTION("GPIO LED + Interrupt Driver with Custom Wait Function");
