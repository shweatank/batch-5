// basic_ioctl_drv.c
// Minimal IOCTL example: send an int from user → kernel → modify → return

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "basic_ioctl"
#define IOCTL_MAGIC 'B'
#define IOCTL_SET_VALUE _IOWR(IOCTL_MAGIC, 1, struct result)

struct result{
	int a;
	int b;
	int sum;
	int diff;
	int mul;
	int div;
	int mod;
	int err;
};

static int major;
//static int kernel_value = 0;

/* ioctl handler */
static long basic_ioctl(struct file *file,
                        unsigned int cmd,
                        unsigned long arg)
{
struct result res;
    switch (cmd) {
    case IOCTL_SET_VALUE:
        /* copy data from user */
        if (copy_from_user(&res, (struct result __user *)arg, sizeof(res)))
            return -EFAULT;

	pr_info("kernel IOCTL_SET_VALUE = 0x%x\n",IOCTL_SET_VALUE);
        pr_info("Kernel: received %d %dfrom user\n", res.a,res.b);
	res.sum=res.a+res.b;
	res.diff=res.a-res.b;
	if(res.a<res.b)
		res.diff=-res.diff;
	res.mul=res.a*res.b;
	if(res.b==0){
		res.err= -EDOM;
		res.div=0;
	}else
		res.div=res.a/res.b;
	if(res.b==0){
		res.err= -EDOM;
		res.mod=0;
	}else
		res.mod=res.a%res.b;

        /* copy data back to user */
        if (copy_to_user((struct result __user *)arg, &res, sizeof(res)))
            return -EFAULT;

        break;

    default:
        return -EINVAL;
    }
    return 0;
}

static struct file_operations fops = {
    .owner          = THIS_MODULE,
    .unlocked_ioctl = basic_ioctl,
};

static int __init basic_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    pr_info("basic_ioctl loaded, major=%d\n", major);
    return 0;
}

static void __exit basic_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    pr_info("basic_ioctl unloaded\n");
}

module_init(basic_init);
module_exit(basic_exit);

MODULE_LICENSE("GPL");
