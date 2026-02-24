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

#define DHT_GPIO_4 4+512//Define GPIO number for DHT sensor data pin.15   4 is BCM GPIO4.16   +512 is used in some Raspberry Pi kernels where GPIO base starts from 512.

#define GPIO_17 17+512// Define GPIO number for Switch pin.17 is BCM GPIO17.+512 because GPIO base offset may start at 512.

#define TIMER_PHY 0xFE003000// Physical base address of the system timer register on Raspberry Pi
#define TIMER_CLO  0x04// Offset for the Timer Counter Lower register (current timer value lower 32 bits)
#define TIMER_SIZE 0x1C// Size of the timer register memory region to be mapped
#define MSEC 1000// Value used to represent 1 millisecond (1000 microseconds)

/* Structure to store DHT sensor data */
//Workqueue handler function.static void delay_us(unsigned int us)
struct dht_data {
	int temperature;//Variable to store temperature value from DHT
	int humidity;// Variable to store humidity value from DHT
};

static int c;
struct dht_data *data;//Variable to store IRQ number. GPIO is converted to IRQ using gpio_to_irq().
static int irq;// Pointer to workqueue structure.This represents a custom workqueue created using create_workqueue().
static struct workqueue_struct *my_wq;//This links the work function (sensor_work) to the workqueue.
static struct work_struct my_work;//Workqueue handler function.static void delay_us(unsigned int us)

static void __iomem *timer_base;
static void delay_us(unsigned int us)// Function to create a delay in microseconds (us)
{
	u32 start, now; // Declare two 32-bit variables to store timer values
	start = readl(timer_base + TIMER_CLO);
	// Read current timer value and store it as start time
	while (1)
		// Start an infinite loop to wait until required time passes
	{
		now = readl(timer_base + TIMER_CLO); // Read current timer value again
		if ((now - start) >= us)// Check if the required microseconds delay has passed
			break;
		// If delay time completed, exit loop
	}
}

static void sensor_work(struct work_struct *work)// Work structure.This links the work function (sensor_work) to the workqueue.
{
	int i, j;
	u8 buf[5] = {0};// Create a 5-byte buffer to store 40 bits (5 bytes) from DHT sensor.Initialize all bytes to 0.

	gpio_direction_output(DHT_GPIO_4, 0);//Set GPIO4 as OUTPUT and make it LOW.
					     // This tells the DHT sensor that we want to start communication.
	msleep(18);//Keep the pin LOW for at least 18 milliseconds.
	gpio_set_value(DHT_GPIO_4, 1);// Make the pin HIGH again.
	udelay(30);// Wait for 30 microseconds.This prepares to switch the pin to input mode.
	gpio_direction_input(DHT_GPIO_4);// Change GPIO4 to INPUT mode.Now we wait for the sensor to respond.
	if (gpio_get_value(DHT_GPIO_4))
		pr_info("GPIO_4 error in sensor work\n");//After switching to input, sensor should pull line LOW.If it is still HIGH, something is wrong. Print error message.

	while (!gpio_get_value(DHT_GPIO_4));// Wait until the sensor makes the pin HIGH. Sensor first pulls LOW (response signal), then makes it HIGH.

	while (gpio_get_value(DHT_GPIO_4));// Wait until the sensor pulls the pin LOW again.This completes the sensor response phase.

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
	// Check whether calculated checksum matches received checksum
	if (((buf[0] + buf[1] + buf[2] + buf[3]) & 0xFF) != buf[4])
		pr_info("sensor error\n");// Print error message if checksum is wrong

	sensor_data->humidity = buf[0];// Store humidity value from buffer into structure
	sensor_data->temperature = buf[2];// Store temperature value from buffer into structure
	pr_info("temp=%d C humidity=%d%%......c=%d*/\n",sensor_data->temperature,sensor_data->humidity,++c);
	enable_irq(irq);// Re-enable interrupt after sensor reading is completed
}
static irqreturn_t GPIO_isr(int irq, void *dev_id)
{
	pr_info("/*........GPIO interrrupt triggered\n");
	queue_work(my_wq, &my_work);// Add the work (my_work) to the workqueue (my_wq) so it will execute later in process context
	disable_irq_nosync(irq); //avoiding multiple triggers
	return IRQ_HANDLED;
}

static int __init GPIO_init(void){
	if(!gpio_is_valid(GPIO_17))//  checking 17 present or not
		pr_err("Invalid GPIO 17\n");
	int rt=gpio_request(GPIO_17,"button"); // 17 as button
	if(rt)
		pr_info("gpio req failed\n");
	gpio_direction_input(GPIO_17); //set as input
	irq=gpio_to_irq(GPIO_17); //gpio number to irq  number
	int ret=request_irq(irq,GPIO_isr,IRQF_TRIGGER_RISING,"GPIO_ISR",NULL);// Register an interrupt handler (GPIO_isr) for this IRQ, triggered on rising edge, and store the result in ret
	if(ret){
		pr_err("request irq failed\n");
		return ret;
	}

	if(!gpio_is_valid(DHT_GPIO_4))
		pr_err("GPIO 4 invalid\n");

	/* GPIO request */
	ret = gpio_request(DHT_GPIO_4, "dht11_gpio");//set as sensor data pin
	if (ret) {
		pr_err("GPIO request failed\n");
		return ret;
	}
	/* Create Workqueue */
	my_wq = create_singlethread_workqueue("dht_wq");// Create a workqueue named "dht_wq" that runs tasks in a single dedicated kernel thread
	if (!my_wq)
		return -ENOMEM;

	INIT_WORK(&my_work, sensor_work);// Initialize the work structure and link it with the function sensor_work
	sensor_data=kmalloc(sizeof(*sensor_data),GFP_KERNEL);// Allocate kernel memory for sensor_data structure using normal sleeping allocation
	gpio_direction_output(DHT_GPIO_4, 1); //set as output for sensor data pin
	pr_info("module loaded\n");
	return 0;
}

static void __exit GPIO_exit(void){
	disable_irq(irq);// Disable the interrupt so it does not trigger again during cleanup

	gpio_set_value(GPIO_17, 0);// Set GPIO 17 output value to LOW before releasing it

	gpio_free(GPIO_17);// Release GPIO 17 so the system can use it again

	pr_info("GPIO_17 freed\n");// Print message that GPIO 17 is released

	gpio_free(DHT_GPIO_4);// Release GPIO 4 (DHT pin)

	pr_info("GPIO_4 freed\n");// Print message that GPIO 4 is released

	free_irq(irq, NULL);// Unregister and free the interrupt handler

	flush_workqueue(my_wq);// Wait until all pending work in the workqueue is finished

	destroy_workqueue(my_wq);// Destroy the workqueue and free its resources

	kfree(sensor_data);// Free the dynamically allocated memory for sensor data

	pr_info("module exit\n");
	// Print message indicating module is unloaded successfully
}


module_init(GPIO_init);
module_exit(GPIO_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("team2");
MODULE_DESCRIPTION("GPIO + DHT11 + workqueue ");
