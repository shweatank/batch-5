#include<linux/kernel.h> // init exit functions
#include<linux/fs.h>   // file operation		  
#include<linux/init.h>
#include<linux/kthread.h>
#include<linux/module.h>
#include<linux/wait.h>
#include<linux/err.h>

static int major=0;
DECLARE_WAIT_QUEUE_HEAD(wq);

static struct task_struct *wait_thread;

int flag=0;


#define DEVICE_NAME "basic_wait"

static  int wait_function(void *p)
{

	   while(!kthread_should_stop())
	   {
		    pr_info("waiting for an event:\n");
		    wait_event_interruptible(wq,flag!=0);

		    pr_info("waiting event is complted: %d\n",flag);

		    flag=0;

	   }

	   return 0;

}

static int my_open(struct inode *inode,struct file *file)
{
	pr_info("open called:\n");
	return 0;
}

static int my_release(struct inode *inode,struct file *file)
{
	pr_info("close called:\n");
	return 0;
}

static ssize_t my_write(struct file *file ,const char __user *buf,size_t len,loff_t *offset)
{
	pr_info("write called:\n");
	return len;
}

static ssize_t  my_read(struct file *file ,char __user *buf,size_t len,loff_t *offset)
{
	pr_info("read called:\n");
	flag=1;
	return len;
}
struct file_operations fops={
	.owner=THIS_MODULE,
	.open=my_open,
	.write=my_write,
	.read=my_read,
	.release=my_release,
};
static int __init wait_init(void)
{

	 major=register_chrdev(0,DEVICE_NAME,&fops);
	 if(major<0)
	 {
		  pr_info("failed to allocate major");
		    return major;
	 }
         pr_info("major=%d\n",major);

	 pr_info("driver is loaded:\n");

        wait_thread=kthread_create(wait_function,0,"my_function");
	if(wait_thread)
	{
		  pr_info("thread creation failed:\n");
		  
	}
	  pr_info("thread created successfully:\n");
	   wake_up_process(wait_thread);

	 return 0;
}

static void __exit wait_exit(void)
{
	 unregister_chrdev(major,DEVICE_NAME);
	 wake_up_interruptible(&wq);

	 pr_info("driver removed:\n");


}
module_init(wait_init);
module_exit(wait_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX <embetronicx@gmail.com>");
MODULE_DESCRIPTION("Simple linux driver (Waitqueue Dynamic method)");
MODULE_VERSION("1.8");
