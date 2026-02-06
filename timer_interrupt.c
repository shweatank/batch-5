#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/hrtimer.h>
#include<linux/ktime.h>
#include<linux/smp.h>

static struct hrtimer timer_var;
static ktime_t time_period;
static unsigned long timer_cnt;

static enum hrtimer_restart timer_callback(struct hrtimer *timer)
{
	timer_cnt++;
	pr_info("timer ticked:%lu  cpu=%d\n",timer_cnt,smp_processor_id());
	hrtimer_forward_now(timer,time_period);
	return HRTIMER_RESTART;
}
static int __init hrtimer_init_fun(void)
{
	u64 period_ns=1000000000ULL; //1sec in ns
	pr_info("hrtimer_init\n");
	time_period=ktime_set(0,period_ns);
	hrtimer_init(&timer_var,CLOCK_MONOTONIC,HRTIMER_MODE_REL);
	//hrtimer_init(&timer_var);
	//timer_var.base = HRTIMER_BASE_MONOTONIC; //set the clock base

	timer_var.function=timer_callback;
	hrtimer_start(&timer_var,time_period,HRTIMER_MODE_REL);
	pr_info("hrtimer is initialised and strated\n");
	return 0;
}
static void __exit hrtimer_exit(void)
{
	pr_info("hrtimer exit\n");
	int ret=hrtimer_cancel(&timer_var);
	pr_info("hrtimer is canceled ret=%d   (1..means..callback is running))\n",ret);

}

module_init(hrtimer_init_fun);
module_exit(hrtimer_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("prasanna");
MODULE_DESCRIPTION("timer interrupt");
MODULE_VERSION("1.0");
