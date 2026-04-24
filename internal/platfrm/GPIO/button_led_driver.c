#include<linux/module.h>
#include<linux/of.h>
#include<linux/gpio.h>
#include<linux/of_gpio.h>
#include<linux/platform_device.h>
#include<linux/interrupt.h>
#include<linux/delay.h>

#define LED_27 27+512
static struct gpio_desc *button_num;
static int irq_num,flag;

static irqreturn_t isr_handler(int irq,void *devm)
{
udelay(60);
   flag^=1;
   if(flag==1)
   gpio_set_value(LED_27,1);
  else
   gpio_set_value(LED_27,0);

	pr_info("in isr function\n");
	return IRQ_HANDLED;
}

static int gpio_probe(struct platform_device *gpio_button)
{
	struct device *dev=&gpio_button->dev;
	pr_info("probe function called\n");
	button_num=devm_gpiod_get(dev,NULL,0);
	if(IS_ERR(button_num))
	{
		pr_info("invalid gpio\n");
		return -EINVAL;
	}
	irq_num=gpiod_to_irq(button_num);
	if(irq_num<0)
	{
		pr_info("irq failed to get the irq number\n");
		return irq_num;
	}
	if(devm_request_irq(dev,irq_num,isr_handler,IRQF_TRIGGER_FALLING,"isr_handler",NULL))
	{
		pr_info("failed to register irq\n");
		return -1;
	}
        if(!gpio_is_valid(LED_27)) //checking GPIO27present or not
          pr_err("GPIO17invalid\n");
int ret;
	/* GPIO request */
	ret = gpio_request(LED_27, "LED_27");//setting GPIO27 as LED
	if (ret) {
		pr_err("GPIO request failed\n");
		return ret;
	}
  gpio_direction_output(LED_27,0);
	pr_info("button irq registered\n");
	return 0;
}
static void gpio_remove(struct platform_device *gpio_buttom)
{
gpio_set_value(LED_27,0);
gpio_free(LED_27);
	pr_info("module removed successfully\n");
}

static const struct of_device_id button_gpio_table[]={
	{.compatible="raspi,gpio_button",},
	{},
};

MODULE_DEVICE_TABLE(of,button_gpio_table);
static struct platform_driver button_gpio_declare={
	.probe=gpio_probe,
	.remove=gpio_remove,
	.driver={
		.name="gpio_button_driver",
		.of_match_table=button_gpio_table,
	},
};
module_platform_driver(button_gpio_declare);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("saikumar");
