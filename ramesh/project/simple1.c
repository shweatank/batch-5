#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/interrupt.h>
#include<linux/io.h>
#include<linux/gpio.h>

#define GPIO 17+512
static int irq;

static irqreturn_t GPIO_isr(int irq, void *dev_id){
	pr_info("in ISR of GPIO\n");
	pr_info("interrrupt triggered\n");
return IRQ_HANDLED;
}

static int __init GPIO_init(void){
	if(!gpio_is_valid(GPIO))
		pr_err("Invalid GPIO\n");
	int rt=gpio_request(GPIO,"button");
	if(rt)
		pr_info("gpio req failed\n");
	gpio_direction_input(GPIO);
	irq=gpio_to_irq(GPIO);
	int ret=request_irq(irq,GPIO_isr,IRQF_TRIGGER_RISING,"GPIO_ISR",NULL);
	if(ret){
		pr_err("request irq failed\n");
		return ret;
	}
return 0;
}

static void __exit GPIO_exit(void){
	pr_info("in exit\n");
	gpio_set_value(GPIO,0);
	gpio_free(GPIO);
	free_irq(irq,NULL);
}


module_init(GPIO_init);
module_exit(GPIO_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("team3");
MODULE_DESCRIPTION("Simple IRQ ");
