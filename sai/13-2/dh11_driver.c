#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define GPIO_DHT11 4   // GPIO pin (BCM numbering, GPIO4)

static int major;
static char result[5]; // humidity_int, humidity_dec, temp_int, temp_dec, checksum

static int dht11_read_data(void)
{
    int i, j = 0;
    unsigned char data[5] = {0};

    gpio_direction_output(GPIO_DHT11, 0);
    msleep(20); // start signal
    gpio_set_value(GPIO_DHT11, 1);
    udelay(30);
    gpio_direction_input(GPIO_DHT11);

    // wait for sensor response
    while (gpio_get_value(GPIO_DHT11));
    while (!gpio_get_value(GPIO_DHT11));
    while (gpio_get_value(GPIO_DHT11));

    for (i = 0; i < 40; i++) {
        while (!gpio_get_value(GPIO_DHT11));
        udelay(30);

        if (gpio_get_value(GPIO_DHT11))
            data[j] = (data[j] << 1) | 1;
        else
            data[j] = (data[j] << 1);

        if ((i + 1) % 8 == 0)
            j++;

        while (gpio_get_value(GPIO_DHT11));
    }

    memcpy(result, data, 5);

    return 0;
}

static ssize_t dht11_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    char out[64];
    dht11_read_data();

    sprintf(out, "Humidity=%d.%d%% Temp=%d.%dC\n",
            result[0], result[1], result[2], result[3]);

   if(copy_to_user(buf, out, strlen(out)))
   {
	   return -EFAULT;
   }
    return strlen(out);
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = dht11_read,
};

static int __init dht11_init(void)
{
    major = register_chrdev(0, "dht11_drv", &fops);
    gpio_request(GPIO_DHT11, "dht11");

    printk(KERN_INFO "DHT11 Driver Loaded. Major=%d\n", major);
    return 0;
}

static void __exit dht11_exit(void)
{
    gpio_free(GPIO_DHT11);
    unregister_chrdev(major, "dht11_drv");
    printk(KERN_INFO "DHT11 Driver Removed\n");
}

module_init(dht11_init);
module_exit(dht11_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("DHT11 Sensor Driver");
