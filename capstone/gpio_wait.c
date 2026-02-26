#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/wait.h>

#define GPIO_PIN 17
#define DEVICE_NAME "gpio_wait"

static int irq_number;
static int major;
static int gpio_flag = 0;   // Condition variable

// Wait queue
DECLARE_WAIT_QUEUE_HEAD(gpio_wait_queue);

// Interrupt Handler
static irqreturn_t gpio_irq_handler(int irq, void *dev_id)
{
    int value = gpio_get_value(GPIO_PIN);

    if (value == 1) {   // VCC detected (HIGH)
        gpio_flag = 1;
        printk(KERN_INFO "GPIO HIGH detected! Waking up process\n");
        wake_up_interruptible(&gpio_wait_queue);
    }

    return IRQ_HANDLED;
}

// Blocking Read Function
static ssize_t gpio_read(struct file *file, char __user *buf,
                         size_t len, loff_t *offset)
{
    printk(KERN_INFO "Process going to sleep waiting for GPIO HIGH...\n");

    // Sleep until gpio_flag becomes 1
    wait_event_interruptible(gpio_wait_queue, gpio_flag == 1);

    printk(KERN_INFO "Process woke up! GPIO HIGH received\n");

    gpio_flag = 0; // Reset flag

    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = gpio_read,
};

// Module Init
static int __init gpio_wait_init(void)
{
    printk(KERN_INFO "GPIO WaitQueue Driver Loaded\n");

    // Register char device
    major = register_chrdev(0, DEVICE_NAME, &fops);
    printk(KERN_INFO "Device registered with major %d\n", major);

    gpio_request(GPIO_PIN, "gpio_wait");
    gpio_direction_input(GPIO_PIN);

    irq_number = gpio_to_irq(GPIO_PIN);
    printk(KERN_INFO "GPIO IRQ number = %d\n", irq_number);

    request_irq(irq_number,
                gpio_irq_handler,
                IRQF_TRIGGER_RISING,
                "gpio_wait_irq",
                NULL);

    return 0;
}

// Module Exit
static void __exit gpio_wait_exit(void)
{
    free_irq(irq_number, NULL);
    gpio_free(GPIO_PIN);
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "GPIO WaitQueue Driver Removed\n");
}

module_init(gpio_wait_init);
module_exit(gpio_wait_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sneha");
MODULE_DESCRIPTION("GPIO Interrupt WaitQueue Driver");
