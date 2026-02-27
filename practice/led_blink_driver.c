#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<liunux/uaccess.h>
#include<linux/gpio.h>
#include<linux/init.h>
#include<linux/device.h>

#define DRIVER_NAME "led_gpio_driver"
#define DEVICE_NAME "led_gpio"
#define CLASS_NAME "led"

//gpiobase+17
#define GPIO_LED (17+512) 

static int major;
static struct class *led_class=NULL;
static struct device *led_device=NULL;
static struct fileoperations fops={.owner=THIS_MODULE,.open=led_open,
.release=led_release,.write=led_write};

static int led_open(struct inode *ino,struct file *fp){
	pr_info("LED:Device opened\n");
	return 0;
}
static int led_release(struct inode *ino,struct file *fp){
	pr_info("led_device closed\n");
	return 0;
}
static int led_write(struct file *fp,const char *buffer,size_t size,loff_t offset){

	char msg[2]={0};
	if(size>2)
		size=2;
	if(copy_from_user(msg,buff,size))
		return -EFAULT;
	if(msg[0]=='1')
	{
		gpio_set_value(GPIO_LED,1);
		pr_info("LED ON \n");
	}
	else
	{
		gpio_set_value(GPIO_LED,0);
		pr_info("LED OFF\n");
	}
	return size;
}
static int __init led_init(void)
{
	if(!(gpio_is_valid(GPIO_LED)))
	{
		pr_info("invalid gpio pin\n");
		return -ENODEV;
	}
	if(!(request_gpio(GPIO_LED,DRIVER_NAME)))
	{
		pr_info("cannot request for gpio..it is already in use\n");
		return -EBUSY;
	}
	gpio_direction_output(GPIO_LED,0);
	major=register_chrdev(0,DEVICE_NAME,&fops);
	if(major<0)
	{
		free_gpio(GPIO_LED);
		pr_info("failed to register chrdev\n");
		return -ENOMEM;
	}
	led_class=create_class(CLASS_NAME);
	if(IS_ERR(led_class))
	{
		unregister_chrdev(major,DEVICE_NAME);
		gpio_free(GPIO_LED);
		pr_err("LED:failed to create led_Class\n");
		return PTR_ERR(led_class);
	}
	led_device=device_create(led_class,NULL,MKDEV(major,0),NULL,DEVICE_NAME);
	if(IS_ERR(led_device))
	{
		class_destroy(led_class);
		unregister_chrdev(major,DEVICE_NAME);
		gpio_free(GPIO_LED);
		pr_err("LED:failed create led device\n");
		return PTR_ERR(led_device);
	}
	pr_info("lED: driver loaded..USE /dev/%s\n",DEVICE_NAME);
	return 0;
}
static void __exit led_exit(void)
{
	gpio_set_value(GPIO_LED,0);
	gpio_free(GPIO_LED);
	device_destroy(led_device);
	class_unregister(led_Class);
	class_destroy(led_class);
	unregister_chrdev(major,DEVIVE_NAME);
	pr_info("LED:driver unloaded\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENCE("GPL");
MODULE_AUTHOR("prasanna");
MODULE_DESCRIPTION("simple led blink using gpio");
