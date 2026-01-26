#include<linux/kernel.h>
#include<linux/uaccess.h>
#include<linux/fs.h>
#include<linux/module.h>
#include<linux/ioctl.h>
#define DEV_NAME "/dev/my_ioctl"
struct st{
	int a;
	int b;
	int res;
};
static int major=0;
#define MAJIC_NUM 'B'
#define IOCTL_ADD _IOWR(MAJIC_NUM,1,struct st)
#define IOCTL_SUB _IOWR(MAJIC_NUM,2,struct st)
#define IOCTL_MUL _IOWR(MAJIC_NUM,3,struct st)
#define IOCTL_DIV _IOWR(MAJIC_NUM,4,struct st)

static long my_ioctl(struct file*file,unsigned int cmd,unsigned long arg){

	struct st *v;
	v=kmalloc(sizeof(struct st),GFP_KERNEL);
	if(copy_from_user(v,(struct st __user*)arg,sizeof(struct st)))
		return -EFAULT;

	switch(cmd){

		case IOCTL_ADD:
			v->res=v->a+v->b;break;
		case IOCTL_SUB:
			v->res=v->a-v->b;break;
		case IOCTL_MUL:
			v->res=v->a*v->b;break;
		case IOCTL_DIV:
			if(v->b!=0)
				v->res=v->a/v->b;
			else
				v->res=0;
			break;
	}
	pr_info("Result is %d\n",v->res);
	if(copy_to_user((struct st __user*)arg,v,sizeof(struct st)))
		return -EFAULT;

	kfree(v);
	return 0;
}

static int my_open(struct inode*inode,struct file*file){
	pr_info("File opened\n");
	return 0;
}
static int my_close(struct inode*inode,struct file* file){
	pr_info("file closed\n");
	return 0;
}

static struct file_operations fops={
	.owner=THIS_MODULE,
		.open=my_open,
		.release=my_close,
		.unlocked_ioctl=my_ioctl,
};

static int __init my_init(void){
	major=register_chrdev(0,DEV_NAME,&fops);
	pr_info("Major number is %d\n",major);
	return 0;
}

static void __exit my_exit(void){
	unregister_chrdev(major,DEV_NAME);
	pr_info("driver unloaded\n");
}
module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("KISHORE THE ALMIGHTY");
MODULE_DESCRIPTION("Basic IOCTL driver");
