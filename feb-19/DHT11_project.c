#include<linux/module.h>//used for module_init ,module_exit(),module_license, module_Author
#include<linux/kernel.h>//pr_info ,pr_err,printk()
#include<linux/interrupt.h>//requst_irq() ,free_irq(),IRQ_HANDLED, IRQF_TRIGGER_RISSING
#include<linux/io.h>//ioremap(),readl,writel
#include<linux/gpio.h>//gpio_request(),gpio_free(),gpio_direction_input(),gpio_direction_output(),gpio_to_irq()
#include<linux/delay.h>//msleep(),mdelay(),udelay()
#include<linux/uaccess.h>//
#include<linux/cdev.h>//cdev_init(),cdev_add(),cdev_del()
#include<linux/fs.h>//struct file_operations,alloc_chrdev_region(),register_chrdev(),unregister_chrdev()
#include<linux/workqueue.h>//INIT_WORK(),queue_work(),creat_singlethread_workqueue(),delay_workqueue()
#include<linux/slab.h>//kmalloc
#include<linux/gfp.h>//memory allocation flags
/* Define GPIO number for DHT sensor data pin.
  4 is BCM GPIO4.
  +512 is used in some Raspberry Pi kernels where GPIO base starts from 512.
 */
#define DHT_GPIO_4 4+512
#define GPIO_17 17+512
/* Define GPIO number for Switch pin.
 * 17 is BCM GPIO17.
 * +512 because GPIO base offset may start at 512.
 */

/* Structure to store DHT sensor data */
struct dht_data {
    int temperature;//Variable to store temperature value from DHT
    int humidity;// Variable to store humidity value from DHT
};

static int c;
struct dht_data *data;//Variable to store IRQ number. GPIO is converted to IRQ using gpio_to_irq().
static int irq;// Pointer to workqueue structure.This represents a custom workqueue created using create_workqueue().
static struct workqueue_struct *my_wq;//This links the work function (sensor_work) to the workqueue.
static struct work_struct my_work;//Workqueue handler function.
static void sensor_work(struct work_struct *work)// Work structure.This links the work function (sensor_work) to the workqueue.
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
