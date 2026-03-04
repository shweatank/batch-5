#include<linux/module.h>
#include<linux/uaccess.h>
#include<linux/fs.h>
#include<linux/kernel.h>


#define DEVICE_NAME "calc_ioctl"

#define CALC_IOC_MAGIC 'C'
#define ADD _IOWR(CALC_IOC_MAGIC,1,calc)

static int major;
typedef struct cal_req
{
	int a;
	int b;
	int result;
}calc;

static int calc_unlocked_ioctl(struct file *file,unsigned int cmd,int arg)
{
	calc var;

	if(copy_from_user(&var,__user arg,sizeof(calc)))
	{
		return EFAULT;
	}
      
	switch(cmd)
	{
		case ADD:
		var.result=var.a+var.b;
		break;

	}

	if(copy_to_user(arg,&var,sizeof(calc)))
	{
		return EFAULT;
	}

	return 0;

}
static int calc_open(struct inode *inode,struct file *file)
{
	return 0;
}
static int calc_release(struct inode *inode,struct file *file)
{
	return 0;
}

struct file_operations calc_fops
{
	.owner=THIS_MODULE;
	.open=calc_open;
	.release=calc_release;
	.unlocked_ioctl=calc_unlocked_ioctl;
};

static int __init calc_init(void)
{
	major=register_chrdev(0,DEVICE_NAME,NULL);
	if(major<0)
	{
		pr_info("failed to allocate major:\n");
		return major;
	}

	pr_info("ioctl driver loaded major=%d\n",major);

	return 0;
}

static void __exit calc_exit(void)
{
	unregister_chrdev(major,DEVICE_NAME);
	pr_info("ioctl driver is unloaded successfully:\n");
}

module_init(calc_init);
module_exit(calc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GURUNATH");
MODULE_DESCRIPTION("IOCTL_CALCULATOR DRIVER");






