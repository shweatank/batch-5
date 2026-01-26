#include"kheader.h"

struct calc
{
int a;
int b;
int res;
};

#define DEVICE_NAME "ioctl_driver"
#define MAGIC 100

static int major;
static struct calc k;

#define ADD _IOWR(MAGIC,1,struct calc)
#define SUB _IOWR(MAGIC,2,struct calc)
#define MUL _IOWR(MAGIC,3,struct calc)
#define DIV _IOWR(MAGIC,4,struct calc)
#define MOD _IOWR(MAGIC,5,struct calc)


static long calc_ioctl(struct file *file,unsigned int cmd,unsigned long arg)
{

if(copy_from_user(&k,(struct calc __user*)arg,sizeof(k)))
return -EFAULT;

switch(cmd)
{
	case ADD:
		k.res=k.a+k.b;
		break;
	case SUB:
		k.res=k.a-k.b;
		break;
	case MUL:
		k.res=k.a*k.b;
		break;
	case DIV:
		k.res=k.a/k.b;
		break;
	case MOD:
		k.res=k.a%k.b;
		break;
}
if(copy_to_user((struct calc __user*)arg,&k,sizeof(k)))
return -EFAULT;
return 0;
}

static struct file_operations fops={
   .owner=THIS_MODULE,
   .unlocked_ioctl=calc_ioctl
};

static int __init calc_init(void)
{
major=register_chrdev(0,DEVICE_NAME,&fops);
if(major<0)
{
pr_info("major number is not allocated for %s\n",DEVICE_NAME);
return 0;
}
pr_info("%s loaded\n",DEVICE_NAME);
pr_info("type - sudo mknod /dev/%s c %d 0\n",DEVICE_NAME,major);
return 0;
}
static void __exit calc_exit(void)
{
unregister_chrdev(major,DEVICE_NAME);
pr_info("%s is unloaded\n",DEVICE_NAME);
}

module_init(calc_init);
module_exit(calc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SAI KUMAR");
MODULE_DESCRIPTION("basic ioctl demo program");
