#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>        // For GPIO functions
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/init.h>

#define DRIVER_NAME "led_gpio_driver"
#define DEVICE_NAME "led_gpio1"
#define CLASS_NAME  "led"

// GPIO17 BCM = GPIO_BASE + 17 = 512 + 17 for Raspberry Pi 4
#define GPIO_LED_OUT (17 + 512)
#define GPIO_LED_IN (18 + 512)

static int major;
static struct class*  led_class  = NULL;
static struct device* led_device = NULL;

static int led_open(struct inode *inodep, struct file *filep) {
    pr_info("LED: Device opened\n");
    return 0;
}

static int led_release(struct inode *inodep, struct file *filep) {
    pr_info("LED: Device closed\n");
    return 0;
}

static ssize_t led_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
    char msg[2] = {0};

    if (len > 1)
        len = 1;

    if (copy_from_user(msg, buffer, len))
        return -EFAULT;

    if (msg[0] == '1') {
        gpio_set_value(GPIO_LED_OUT, 1);
        pr_info("LED: ON\n");
    } else if (msg[0] == 0) {
        gpio_set_value(GPIO_LED_OUT, 0);
        pr_info("LED: OFF\n");
    }
    return len;
}

static ssize_t led_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
   // char msg[2] = {0};

    if (len > 1)
        len = 1;

   // if (copy_to_user(buffer, msg, len))
     //   return -EFAULT;

   if (gpio_get_value(GPIO_LED_IN)==1) 
        buffer[0]='1';
else
	buffer[0]='0';
//	pr_info("LED: ON sent\n");
  //  } else if (msg[0] == '0') {
    //    msg[0]=gpio_get_value(GPIO_LED_IN);
      //  pr_info("LED: OFF sent\n");
   

    return len;
}




static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = led_open,
    .write = led_write,
    .read=led_read,
    .release = led_release,
};

static int __init led_init(void) {
    pr_info("LED: Initializing driver...\n");

    // Request the GPIO
    if (!gpio_is_valid(GPIO_LED_OUT)) {
        pr_err("LED: Invalid GPIO_OUT %d\n", GPIO_LED_OUT);
        return -ENODEV;
    }
    if (!gpio_is_valid(GPIO_LED_IN)) {
        pr_err("LED: Invalid GPIO_IN %d\n", GPIO_LED_IN);
        return -ENODEV;
    }

    if (gpio_request(GPIO_LED_OUT, DRIVER_NAME)) {
        pr_err("LED: Failed to request GPIO_OUT %d\n", GPIO_LED_OUT);
        return -EBUSY;
    }
    if (gpio_request(GPIO_LED_IN, DRIVER_NAME)) {
        pr_err("LED: Failed to request GPIO_IN %d\n", GPIO_LED_IN);
        return -EBUSY;
    }

    gpio_direction_output(GPIO_LED_OUT, 0);  // Set as output, initial LOW
    gpio_direction_output(GPIO_LED_IN, 1);  // Set as output, initial LOW

    // Character device registration
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        gpio_free(GPIO_LED_OUT);
        gpio_free(GPIO_LED_IN);
	
        pr_err("LED: Failed to register major number\n");
        return major;
    }

    led_class = class_create(CLASS_NAME);
    if (IS_ERR(led_class)) {
        unregister_chrdev(major, DEVICE_NAME);
        gpio_free(GPIO_LED_OUT);
        gpio_free(GPIO_LED_IN);
        pr_err("LED: Failed to create class\n");
        return PTR_ERR(led_class);
    }

    led_device = device_create(led_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(led_device)) {
        class_destroy(led_class);
        unregister_chrdev(major, DEVICE_NAME);
        gpio_free(GPIO_LED_OUT);
        gpio_free(GPIO_LED_IN);
        pr_err("LED: Failed to create device\n");
        return PTR_ERR(led_device);
    }

    pr_info("LED: Driver loaded. Use /dev/%s\n", DEVICE_NAME);
    return 0;
}

static void __exit led_exit(void) {
    gpio_set_value(GPIO_LED_OUT, 0);  // Turn off LED
    gpio_free(GPIO_LED_OUT);
    gpio_free(GPIO_LED_IN);
	
    device_destroy(led_class, MKDEV(major, 0));
    class_unregister(led_class);
    class_destroy(led_class);
    unregister_chrdev(major, DEVICE_NAME);

    pr_info("LED: Driver unloaded\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("techdhaba");
MODULE_DESCRIPTION("GPIO LED Driver without Device Tree");

