#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/interrupt.h>
#include<linux/io.h>
#include<linux/gpio.h>
#include<linux/delay.h>
#include<linux/uaccess.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/workqueue.h>
#include<linux/slab.h>
#include<linux/gfp.h>

#define DHT_GPIO_4 4+512
#define GPIO_17 17+512

struct dht_data {
    int temperature;
    int humidity;
};
static int c;
struct dht_data *data;
static int irq;
static struct workqueue_struct *my_wq;
static struct work_struct my_work;
static void sensor_work(struct work_struct *work)
{
data=kmalloc(sizeof(*data),GFP_KERNEL);
 int i, j;
    u8 buf[5] = {0};

    /* Start signal */
    gpio_direction_output(DHT_GPIO_4, 0);
    msleep(18);
    gpio_set_value(DHT_GPIO_4, 1);
    udelay(30);
    gpio_direction_input(DHT_GPIO_4);

    /* Response */
    if (gpio_get_value(DHT_GPIO_4))
        pr_info("GPIO_4 error in sensor work\n");

    while (!gpio_get_value(DHT_GPIO_4));
    while (gpio_get_value(DHT_GPIO_4));

    /* Read 40 bits */
    for (j = 0; j < 5; j++) {
        for (i = 0; i < 8; i++) {

            while (!gpio_get_value(DHT_GPIO_4));

            udelay(30);

            if (gpio_get_value(DHT_GPIO_4))
                buf[j] |= (1 << (7 - i));

            while (gpio_get_value(DHT_GPIO_4));
        }
    }

    /* Checksum */
    if (((buf[0] + buf[1] + buf[2] + buf[3]) & 0xFF) != buf[4])
        pr_info("sensor error\n");

    data->humidity = buf[0];
    data->temperature = buf[2];
pr_info("temp=%d C humidity=%d%%......c=%d*/\n",data->temperature,data->humidity,++c);
enable_irq(irq);
}
static irqreturn_t GPIO_isr(int irq, void *dev_id)
{
	pr_info("/*........GPIO interrrupt triggered\n");
    queue_work(my_wq, &my_work);
disable_irq_nosync(irq);
return IRQ_HANDLED;
}

static int __init GPIO_init(void){
	if(!gpio_is_valid(GPIO_17))//  checking 17 present or not
		pr_err("Invalid GPIO\n");
	int rt=gpio_request(GPIO_17,"button"); // 17 as button
	if(rt)
		pr_info("gpio req failed\n");
	gpio_direction_input(GPIO_17); //set as inpuy
	irq=gpio_to_irq(GPIO_17); //gpio number to irq  number
	int ret=request_irq(irq,GPIO_isr,IRQF_TRIGGER_RISING,"GPIO_ISR",NULL);
	if(ret){
		pr_err("request irq failed\n");
		return ret;
	}

	/* GPIO request */
	ret = gpio_request(DHT_GPIO_4, "dht11_gpio");
	if (ret) {
		pr_err("GPIO request failed\n");
		return ret;
	}
/* Create Workqueue */
    my_wq = create_singlethread_workqueue("dht_wq");
    if (!my_wq)
        return -ENOMEM;

    INIT_WORK(&my_work, sensor_work);

	gpio_direction_output(DHT_GPIO_4, 1);
	pr_info("module loaded\n");
	return 0;
}

static void __exit GPIO_exit(void){

	disable_irq(irq);
	gpio_set_value(GPIO_17,0);
	gpio_free(GPIO_17);
	pr_info("GPIO_17 freed\n");
	gpio_free(DHT_GPIO_4);
	pr_info("GPIO_4 freed\n");
	free_irq(irq,NULL);
	flush_workqueue(my_wq);
	destroy_workqueue(my_wq);
	kfree(data);
	pr_info("module exit\n");

}


module_init(GPIO_init);
module_exit(GPIO_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("team2");
MODULE_DESCRIPTION("Simple IRQ ");
