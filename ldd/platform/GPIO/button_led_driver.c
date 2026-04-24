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

static irqreturn_t isr_handler(int irq,void *dev)
{
   flag^=1;
   if(flag==1)
   gpio_set_value(LED_27,1);
  else
   gpio_set_value(LED_27,0);

	pr_info("in isr function\n");
	return IRQ_HANDLED;
}

static int gpio_probe(struct platform_device *p)
{
	pr_info("probe function called\n");
	button_num=devm_gpiod_get(&p->dev,NULL,0);
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
	if(devm_request_irq(&p->dev,irq_num,isr_handler,IRQF_TRIGGER_FALLING,"isr_handler",NULL))
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
static void gpio_remove(struct platform_device *p)
{
	pr_info("module removed successfully\n");
}

static const struct of_device_id button_gpio_table[]={
	{.compatible="gpio_button_led",},
	{},
};

MODULE_DEVICE_TABLE(of,button_gpio_table);
static struct platform_driver gpio_driver={
	.probe=gpio_probe,
	.remove=gpio_remove,
	.driver={
		.name="gpio_button_led_driver",
		.of_match_table=button_gpio_table,
	},
};
module_platform_driver(gpio_driver);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("saikumar");
