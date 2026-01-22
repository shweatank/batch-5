// basic_ioctl_drv.c
// Minimal IOCTL example: send an int from user → kernel → modify → return

#include <linux/module.h>
#include <linux/fs.h>
#include<linux/kernel.h>
#include <linux/uaccess.h>
#include<linux/slab.h>

#define DEVICE_NAME "calc_ioctl"
#define CALC_IOC_MAGIC  'C'

struct calc_req {
    int a;
    int b;
    long result;   // long to reduce overflow risk vs int (still possible)
    int err;       // 0 on success, -EINVAL/-ERANGE/-EDOM etc.
};


#define CALC_IOC_ADD  _IOWR(CALC_IOC_MAGIC, 1, struct calc_req)
#define CALC_IOC_SUB  _IOWR(CALC_IOC_MAGIC, 2, struct calc_req)
#define CALC_IOC_MUL  _IOWR(CALC_IOC_MAGIC, 3, struct calc_req)
#define CALC_IOC_DIV  _IOWR(CALC_IOC_MAGIC, 4, struct calc_req)
#define CALC_IOC_MOD  _IOWR(CALC_IOC_MAGIC, 5, struct calc_req)

static int major;
    struct calc_req *user_value;
/* ioctl handler */
static long calc_ioctl(struct file *file,
		unsigned int cmd,
		unsigned long arg)
{

	if (copy_from_user(user_value,(struct calc_req __user*)arg , sizeof(struct calc_req)))
		return -EFAULT;
	switch (cmd) {
		case CALC_IOC_ADD:
			user_value->result=user_value->a+user_value->b;
			printk(KERN_INFO "%d + %d is %ld\n",user_value->a,user_value->b,user_value->result);
			break;
		case CALC_IOC_SUB:
			user_value->result=user_value->a-user_value->b;
			printk(KERN_INFO "%d - %d is %ld\n",user_value->a,user_value->b,user_value->result);
			break;
		case CALC_IOC_MUL:
			user_value->result=user_value->a*user_value->b;
			printk(KERN_INFO "%d * %d is %ld\n",user_value->a,user_value->b,user_value->result);
			break;
		case CALC_IOC_DIV:
			user_value->result=user_value->a/user_value->b;
			printk(KERN_INFO "%d / %d is %ld\n",user_value->a,user_value->b,user_value->result);
			break;
		case CALC_IOC_MOD:
			user_value->result=user_value->a%user_value->b;
			printk(KERN_INFO "%d %% %d is %ld\n",user_value->a,user_value->b,user_value->result);
			break;

		default:
			return -EINVAL;
	}
	if (copy_to_user((struct calc_req __user *)arg, user_value, sizeof(struct calc_req)))
		return -EFAULT;
	return 0;
}

static struct file_operations fops = {
    .owner          = THIS_MODULE,
    .unlocked_ioctl = calc_ioctl,
};

static int __init basic_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    pr_info("calc_ioctl loaded, major=%d\n", major);
user_value=kmalloc(sizeof(struct calc_req),GFP_KERNEL);
if(!user_value) 
return -1;
printk(KERN_INFO "memory allocated\n");   
    return 0;
}

static void __exit basic_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    pr_info("calc_ioctl unloaded\n");
    kfree(user_value);
    printk(KERN_INFO "memory deallocted\n");
}

module_init(basic_init);
module_exit(basic_exit);

MODULE_LICENSE("GPL");
