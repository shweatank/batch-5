#include<linux/init.h>
#include<linux/mutex.h>
#include<linux/kernel.h>
#include<linux/wait.h>
#include<linux/kthread.h>


int falg1=0,flag2=0;

static int flag=0;


static DECLARE_WAIT_QUEUE_HEAD(wq);

static struct task_struct *task1;

static struct task_struct *task2;



wait_event_interruptable(wq,flag==0);  // ot will sleep untill the condition becomes true;
				       //

