// SPDX-License-Identifier: GPL
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/workqueue.h>
#include <linux/delay.h>

#define IRQ_GPIO   (17 + 512)   // Interrupt pin
#define DHT_GPIO   (4  + 512)   // DHT11 data pin

static int irq;

static struct workqueue_struct *my_wq;
static struct work_struct my_work;

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

    /* Response */
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

/* ================= WORKQUEUE ================= */

static void sensor_work(struct work_struct *work)
{
}

/* ================= ISR ================= */

static irqreturn_t GPIO_isr(int irq, void *dev_id)
{
   
    
struct dht_data data;
    int ret;

    pr_info("Workqueue triggered → Reading DHT11\n");

    ret = dht11_read_data(&data);
    if (ret) {
        pr_err("DHT11 read failed\n");
        return IRQ_HANDLED;
    }

    pr_info("Temperature = %d C  Humidity = %d %%\n",
            data.temperature, data.humidity);

   // queue_work(my_wq, &my_work);
    pr_info("GPIO Interrupt occurred\n");

    return IRQ_HANDLED;
}

/* ================= INIT ================= */

static int __init my_driver_init(void)
{
    int ret;

    pr_info("Driver Init\n");

    /* Request IRQ GPIO */
    if (!gpio_is_valid(IRQ_GPIO)) {
        pr_err("Invalid IRQ GPIO\n");
        return -EINVAL;
    }

    ret = gpio_request(IRQ_GPIO, "irq_gpio");
    if (ret)
        return ret;

    gpio_direction_input(IRQ_GPIO);

    /* Request DHT GPIO */
    if (!gpio_is_valid(DHT_GPIO)) {
        pr_err("Invalid DHT GPIO\n");
        return -EINVAL;
    }

    ret = gpio_request(DHT_GPIO, "dht_gpio");
    if (ret)
        return ret;

    gpio_direction_output(DHT_GPIO, 1);

    /* Create Workqueue */
    my_wq = create_singlethread_workqueue("dht_wq");
    if (!my_wq)
        return -ENOMEM;

    INIT_WORK(&my_work, sensor_work);

    /* IRQ */
    irq = gpio_to_irq(IRQ_GPIO);

    ret = request_irq(irq,
                      GPIO_isr,
                      IRQF_TRIGGER_RISING,
                      "gpio_irq",
                      NULL);

    if (ret) {
        pr_err("IRQ request failed\n");
        return ret;
    }

    pr_info("Driver Loaded Successfully\n");
    return 0;
}

/* ================= EXIT ================= */

static void __exit my_driver_exit(void)
{
    pr_info("Driver Exit\n");
    disable_irq(irq);
    free_irq(irq, NULL);

    flush_workqueue(my_wq);
    destroy_workqueue(my_wq);

    gpio_free(IRQ_GPIO);
    gpio_free(DHT_GPIO);
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Saikumar");
MODULE_DESCRIPTION("GPIO Interrupt + Workqueue + DHT11");

