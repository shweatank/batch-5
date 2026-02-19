#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/init.h>

#define TRIGGER_GPIO 17+512   // GPIO for button/interrupt
#define DHT11_PIN    4+512    // GPIO for DHT11

static int irq_number;
static int dht_humidity = 0;
static int dht_temperature = 0;

// Forward declaration of tasklet
static void dht11_tasklet_func(unsigned long data);
DECLARE_TASKLET(dht11_tasklet, dht11_tasklet_func);

// ISR: only schedules the tasklet
static irqreturn_t gpio_isr(int irq, void *dev_id)
{
    tasklet_schedule(&dht11_tasklet);  // schedule DHT11 read
    return IRQ_HANDLED;
}

// Tasklet function: reads DHT11
static void dht11_tasklet_func(unsigned long data)
{
    int dht_data[5] = {0,0,0,0,0};
    int laststate = 1;
    int counter = 0;
    int j = 0, i;

    // Start signal
    gpio_direction_output(DHT11_PIN, 0);
    udelay(18000); // 18ms
    gpio_direction_output(DHT11_PIN, 1);
    udelay(40);
    gpio_direction_input(DHT11_PIN);

    // Read 40 bits
    for(i=0;i<85;i++){
        counter = 0;
        while(gpio_get_value(DHT11_PIN) == laststate){
            counter++;
            udelay(1);
            if(counter == 255) break;
        }
        laststate = gpio_get_value(DHT11_PIN);

        if(i >= 4 && i % 2 == 0){
            dht_data[j/8] <<= 1;
            if(counter > 16)
                dht_data[j/8] |= 1;
            j++;
        }
    }

    // Check checksum
    if(j >= 40 && dht_data[4] == ((dht_data[0]+dht_data[1]+dht_data[2]+dht_data[3]) & 0xFF)){
        dht_humidity = dht_data[0];
        dht_temperature = dht_data[2];
        printk(KERN_INFO "DHT11 Tasklet: Humidity=%d%% Temp=%dC\n",
               dht_humidity, dht_temperature);
    } else {
        printk(KERN_INFO "DHT11 Tasklet: Failed to read data\n");
    }
}

// Module init
static int __init dht11_init(void)
{
    int ret;

    if(!gpio_is_valid(TRIGGER_GPIO)){
        printk(KERN_ALERT "Invalid trigger GPIO\n");
        return -ENODEV;
    }

    // Request trigger GPIO
    gpio_request(TRIGGER_GPIO, "gpio_trigger");
    gpio_direction_input(TRIGGER_GPIO);

    // Request DHT11 GPIO
    if(!gpio_is_valid(DHT11_PIN)){
        printk(KERN_ALERT "Invalid DHT11 GPIO\n");
        return -ENODEV;
    }
    gpio_request(DHT11_PIN, "dht11_gpio");

    // Map GPIO to IRQ
    irq_number = gpio_to_irq(TRIGGER_GPIO);
    printk(KERN_INFO "GPIO mapped to IRQ: %d\n", irq_number);

    // Request IRQ (rising edge)
    ret = request_irq(irq_number, gpio_isr, IRQF_TRIGGER_RISING, "dht11_gpio_irq", NULL);
    if(ret){
        printk(KERN_ALERT "Failed to request IRQ\n");
        return ret;
    }

    printk(KERN_INFO "DHT11 Tasklet Module Loaded\n");
    return 0;
}

// Module exit
static void __exit dht11_exit(void)
{
    free_irq(irq_number, NULL);
    gpio_free(TRIGGER_GPIO);
    gpio_free(DHT11_PIN);
    tasklet_kill(&dht11_tasklet);
    printk(KERN_INFO "DHT11 Tasklet Module Unloaded\n");
}

module_init(dht11_init);
module_exit(dht11_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("team3");
MODULE_DESCRIPTION("DHT11 Kernel Module using Tasklet");

