#include<linux/module.h> //basic module macros
#include<linux/init.h> //__init,__exit
#include<linux/fs.h>  //fileopns,register_chrdev
#include<linux/kernel.h>
#include<linux/interrupt.h>

#define DRIVER_NAME "irq_driver"
#define IRQ_NUM 1 //keyboard interrupt

struct my_irq_dev{
	int counter;
	struct device *ptr;
};
static struct my_irq_dev var;
static irqreturn_t irq_handler(int irq,void *devid)
{
	var.counter++;
	pr_info("%s: IRQ %d recieved..counter val:%d\n",DRIVER_NAME,IRQ_NUM,var.counter);

	return IRQ_HANDLED;
}
static int __init irq_demo_init(void)
{
	int ret;
	var.ptr=(var.ptr); //for demo
	var.counter=0;
	ret=request_irq(IRQ_NUM,irq_handler,IRQF_SHARED,DRIVER_NAME,&var);	
	if(ret)
	{
		pr_err("%s failed to request IRQ:%d\n",DRIVER_NAME,IRQ_NUM);
		return ret;
	}
	pr_info("%s: IRQ %d registered successfully\n",DRIVER_NAME,IRQ_NUM);
	return 0;
}

static void __exit irq_demo_exit(void)
{
	free_irq(IRQ_NUM,&var);
	pr_info("cleaned up %s\n",DRIVER_NAME);
}
module_init(irq_demo_init);
module_exit(irq_demo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("prasanna");
MODULE_DESCRIPTION("demo_isr_driver");
