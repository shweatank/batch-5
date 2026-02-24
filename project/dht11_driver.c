#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/delay.h>

#define GPIO 17+512          // GPIO17 for button/interrupt
#define DHT11_PIN 4      // GPIO4 for DHT11

static int irq;
static int dht_humidity = 0;
static int dht_temperature = 0;

static irqreturn_t GPIO_isr(int irq, void *dev_id){
    int data[5] = {0,0,0,0,0};
    int laststate = 1;
    int counter = 0;
    int j = 0, i;

    printk(KERN_INFO "GPIO ISR triggered\n");

    // Send start signal to DHT11
    gpio_direction_output(DHT11_PIN, 0);
    udelay(18000);  // 18ms
    gpio_direction_output(DHT11_PIN, 1);
    udelay(40);
    gpio_direction_input(DHT11_PIN);

    // Read 40 bits
    for(i=0;i<85;i++){
        counter=0;
        while(gpio_get_value(DHT11_PIN)==laststate){
            counter++;
            udelay(1);
            if(counter==255)
                break;
        }
        laststate = gpio_get_value(DHT11_PIN);

        if(i>=4 && i%2==0){
            data[j/8] <<= 1;
            if(counter>16)
                data[j/8] |= 1;
            j++;
        }
    }

    // Validate checksum
    if(j>=40 && data[4] == ((data[0]+data[1]+data[2]+data[3]) & 0xFF)){
        dht_humidity = data[0];
        dht_temperature = data[2];
        printk(KERN_INFO "DHT11 ISR: Humidity=%d%% Temp=%dC\n",
                dht_humidity, dht_temperature);
    } else {
        printk(KERN_INFO "DHT11 ISR: Failed to read\n");
    }

    return IRQ_HANDLED;
}

static int __init GPIO_init(void){
    int ret;

    if(!gpio_is_valid(GPIO)){
        pr_err("Invalid GPIO\n");
        return -ENODEV;
    }

    gpio_request(GPIO, "button");
    gpio_direction_input(GPIO);

    irq = gpio_to_irq(GPIO);
    printk(KERN_INFO "GPIO mapped to IRQ: %d\n", irq);

    ret = request_irq(irq, GPIO_isr, IRQF_TRIGGER_RISING, "gpio_isr", NULL);
    if(ret){
        pr_err("request_irq failed\n");
        return ret;
    }

    printk(KERN_INFO "Module loaded, waiting for interrupt\n");
    return 0;
}

static void __exit GPIO_exit(void){
    free_irq(irq, NULL);
    gpio_free(GPIO);
    printk(KERN_INFO "Module unloaded\n");
}

module_init(GPIO_init);
module_exit(GPIO_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("team3");
MODULE_DESCRIPTION("GPIO Interrupt DHT11 ISR Example");

