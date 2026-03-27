#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/interrupt.h>
#include<linux/io.h>
#include<linux/gpio.h>
#include<linux/workqueue.h>
#include<linux/delay.h>

#define GPIO 17+512
#define DHT11_PIN 4+512
#define MAX_TIMINGS 85

static int irq;
static struct workqueue_struct *my_wq;
static struct work_struct my_work;

static void sensor_data(struct work_struct *work){

	int laststate = 1;
	int counter = 0;
	int j = 0, i;
	int data[5] = {0,0,0,0,0};
	pr_info("Entered workqueue\n");
	// Send start signal
	gpio_direction_output(DHT11_PIN, 0);  // GPIO4
	udelay(18000); // 18 ms
	gpio_direction_output(DHT11_PIN, 1);
	udelay(40);    // 40 us
	gpio_direction_input(DHT11_PIN);

	// Read the data
	for(i=0;i<MAX_TIMINGS;i++) {
		counter = 0;
		while (gpio_get_value(DHT11_PIN) == laststate) {
			counter++;
			udelay(1); // 1 us
			if(counter == 255)
				break;
		}
		laststate = gpio_get_value(DHT11_PIN);

		if(i >= 4 && i % 2 == 0){
			data[j/8] <<= 1;
			if(counter > 16)
				data[j/8] |= 1;
			j++;
		}
	}

	// Check checksum
	if(j >= 40 && data[4] == ((data[0]+data[1]+data[2]+data[3]) & 0xFF)) {
		pr_info("Humidity = %d.%d %% Temperature = %d.%d *C\n",
				data[0], data[1], data[2], data[3]);
	} else {
		pr_info("Failed to read from DHT11\n");
	}
	pr_info("entered work_queue\n");

}


static irqreturn_t GPIO_isr(int irq, void *dev_id){
	pr_info("in ISR of GPIO\n");
	pr_info("interrrupt triggered\n");
	queue_work(my_wq,&my_work);
	//return IRQ_HANDLED;
	return IRQ_WAKE_THREAD;
}

static int __init GPIO_init(void){
	if(!gpio_is_valid(GPIO))
		pr_err("Invalid GPIO\n");
	int rt=gpio_request(GPIO,"button");
	if(rt)
		pr_info("gpio req failed\n");
	gpio_direction_input(GPIO);
	/*my_wq=create_singlethread_workqueue("team4");
	  if(!my_wq)
	  return -ENOMEM;
	  INIT_WORK(&my_work,sensor_data);*/
	irq=gpio_to_irq(GPIO);
	int ret=request_irq(irq,GPIO_isr,IRQF_TRIGGER_RISING,"GPIO_ISR",NULL);
	if(ret){
		pr_err("request irq failed\n");
		return ret;
	}
	my_wq=create_singlethread_workqueue("team4");
	if(!my_wq)
		return -ENOMEM;
	INIT_WORK(&my_work,sensor_data);
	//gpio_direction_input(GPIO);
	//	irq=gpio_to_irq(GPIO);
	pr_info("Module loaded succeefully\n");
	return 0;
}

static void __exit GPIO_exit(void){
	pr_info("in exit\n");
	gpio_set_value(GPIO,0);
	gpio_free(GPIO);
	free_irq(irq,NULL);
	flush_workqueue(my_wq);
	destroy_workqueue(my_wq);
}


module_init(GPIO_init);
module_exit(GPIO_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("team4");
MODULE_DESCRIPTION("Simple IRQ ");
