// SPDX-License-Identifier: GPL
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "dht11"
#define CLASS_NAME  "dht"

#define DHT_GPIO 4+512   // BCM GPIO4

static dev_t devno;
static struct cdev dht_cdev;
static struct class *dht_class;

struct dht_data {
    int temperature;
    int humidity;
};

static int dht11_read_data(struct dht_data *data)
{
    int i, j;
    u8 buf[5] = {0};

    /* Start signal */
    gpio_direction_output(DHT_GPIO, 0);
    msleep(18);
    gpio_set_value(DHT_GPIO, 1);
    udelay(30);
    gpio_direction_input(DHT_GPIO);

    /* Sensor response */
    if (gpio_get_value(DHT_GPIO))
        return -EIO;

    while (!gpio_get_value(DHT_GPIO));
    while (gpio_get_value(DHT_GPIO));

    /* Read 40 bits */
    for (j = 0; j < 5; j++) {
        for (i = 0; i < 8; i++) {

            while (!gpio_get_value(DHT_GPIO));

            udelay(30);

            if (gpio_get_value(DHT_GPIO))
                buf[j] |= (1 << (7 - i));

            while (gpio_get_value(DHT_GPIO));
        }
    }

    /* Checksum */
    if (((buf[0] + buf[1] + buf[2] + buf[3]) & 0xFF) != buf[4])
        return -EIO;

    data->humidity = buf[0];
    data->temperature = buf[2];

    return 0;
}

static ssize_t dht_read(struct file *f, char __user *buf,
                        size_t len, loff_t *off)
{
    struct dht_data data;
    char out[32];
    int ret;

    if (*off > 0)
        return 0;

    ret = dht11_read_data(&data);
    if (ret)
        return ret;

    snprintf(out, sizeof(out),
             "Temp=%d C Hum=%d %%\n",
             data.temperature, data.humidity);

    if (copy_to_user(buf, out, strlen(out)))
        return -EFAULT;

    *off = strlen(out);
    return *off;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read  = dht_read,
};

static int __init dht_init(void)
{
    int ret;

    /* Allocate device number */
    ret = alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME);
    if (ret)
        return ret;

    cdev_init(&dht_cdev, &fops);
    cdev_add(&dht_cdev, devno, 1);

    dht_class = class_create(CLASS_NAME);
    device_create(dht_class, NULL, devno, NULL, DEVICE_NAME);

    /* GPIO request */
    ret = gpio_request(DHT_GPIO, "dht11_gpio");
    if (ret) {
        pr_err("GPIO request failed\n");
        return ret;
    }

    gpio_direction_output(DHT_GPIO, 1);

    pr_info("DHT11 driver loaded device:%s\n",DEVICE_NAME);
    return 0;
}

static void __exit dht_exit(void)
{
    gpio_free(DHT_GPIO);

    device_destroy(dht_class, devno);
    class_destroy(dht_class);

    cdev_del(&dht_cdev);
    unregister_chrdev_region(devno, 1);

    pr_info("DHT11 driver removed\n");
}

module_init(dht_init);
module_exit(dht_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Saikumar");
MODULE_DESCRIPTION("DHT11 Driver using gpio_request");

