#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/interrupt.h>
#include <linux/smp.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("prasanna");
MODULE_DESCRIPTION("hrtimer schedules a tasklet (bottom-half demo)");
MODULE_VERSION("1.0");

static struct hrtimer timer_var;
static ktime_t time_period;
static unsigned long timer_cnt;

static void tasklet_fun(struct tasklet_struct *t )
{
	pr_info("in tasklet function\n");
	pr_info("timer cnt:%lu cpu:%d",timer_cnt,smp_processor_id());
}
DECLARE_TASKLET(timer_tasklet,tasklet_fun);
static enum hrtimer_restart timer_cb(struct hrtimer *timer)
{
	timer_cnt++;
	pr_info("hrtimer ticked and val :%lu cpu:%d\n",timer_cnt,smp_processor_id());
	tasklet_schedule(&timer_tasklet);
	pr_info("tasklet scheduled\n");
	hrtimer_forward_now(timer,time_period);
	return HRTIMER_RESTART;
}
static int __init hrtimer_init_fun(void)
{
	u64 period_ns=500*1000*1000; //500ms
	timer_cnt=0;
	time_period=ktime_set(0,period_ns);
	hrtimer_init(&timer_var,CLOCK_MONOTONIC,HRTIMER_MODE_REL);
	timer_var.function=timer_cb;
	pr_info("timer and tasklet initialised\n");
	hrtimer_start(&timer_var,time_period,HRTIMER_MODE_REL);
	pr_info("hrtimer started\n");
	return 0;	
}
static void __exit hrtimer_exit(void)
{
	int ret=hrtimer_cancel(&timer_var);
	tasklet_kill(&timer_tasklet);
	pr_info("timer stopped res=%d and tasklet killed\n",ret);
}
module_init(hrtimer_init_fun);
module_exit(hrtimer_exit);
