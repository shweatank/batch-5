#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/interrupt.h>
#include<linux/io.h>
#include<linux/gpio.h>
#include<linux/workqueue.h>
#include<linux/delay.h>
#include<linux/kmod.h>
#define GPIO 17+512

static int irq;
static struct workqueue_struct *my_wq;
static struct work_struct my_work;
/*
static void sensor_data(struct work_struct *work){
	char *argv[]={"/home/team4/kishore/project/dummy","hello",NULL};
	char *envp[]={"HOME=/","TERM=linux","PATH=/sbin:/bin:/usr/sbin:/usr/bin",NULL};
	int ret=call_usermodehelper(argv[0],argv,envp,UMH_WAIT_PROC);
	if(ret==0){
		pr_info("Message from kernel is succefull\n");
	}
	else
		pr_info("Message from kernel is unsuccessfull\n");
	pr_info("entered work_queue\n");

}*/

static void sensor_data(struct work_struct *work)
{
    char *argv[] = { "/home/team4/kishore/project/dummy", "Hello", NULL };
    char *envp[] = { "HOME=/", "TERM=linux", "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };

    int ret = call_usermodehelper(argv[0], argv, envp, UMH_WAIT_PROC);

    if (ret == 0)
        pr_info("Message from kernel is successful\n");
    else
        pr_err("Message from kernel is unsuccessful: %d\n", ret);

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
	/*my_wq=create_singlethread_workqueue("team2");
	  if(!my_wq)
	  return -ENOMEM;
	  INIT_WORK(&my_work,sensor_data);*/
	irq=gpio_to_irq(GPIO);
	int ret=request_irq(irq,GPIO_isr,IRQF_TRIGGER_RISING,"GPIO_ISR",NULL);
	if(ret){
		pr_err("request irq failed\n");
		return ret;
	}
	my_wq=create_singlethread_workqueue("team2");
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
MODULE_AUTHOR("team3");
MODULE_DESCRIPTION("Simple IRQ ");
