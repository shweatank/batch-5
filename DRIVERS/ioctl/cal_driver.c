// basic_ioctl_drv.c
// Minimal IOCTL example: send an int from user → kernel → modify → return

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

static struct st{
int a;
int b;
int res;
};
#define DEVICE_NAME "basic_ioctl"
#define IOCTL_MAGIC 'B'
#define IOCTL_ADD _IOWR(IOCTL_MAGIC, 1, struct st)
#define IOCTL_SUB _IOWR(IOCTL_MAGIC, 2, struct st)
#define IOCTL_MUL _IOWR(IOCTL_MAGIC, 3, struct st)
#define IOCTL_DIV _IOWR(IOCTL_MAGIC, 4, struct st)

static struct st kernel_value;
/* ioctl handler */
static long basic_ioctl(struct file *file,
                        unsigned int cmd,
                        struct st *v)
{
	struct st user_value;

	if (copy_from_user(&user_value, v , sizeof(struct st)))
		return -EFAULT;
	printk(KERN_INFO "Kernel recived %d and %d from user\n",user_value.a,user_value.b);
	switch (cmd) {
		case IOCTL_ADD:
			v->res=v->a+v->b;
			break;
		case IOCTL_SUB:
			v->res=v->a-v->b;
			break;
		case IOCTL_MUL:
			v->res=v->a*v->b;
			break;
		case IOCTL_DIV:
			v->res=v->a/v->b;
			break;

		default:
			return -EINVAL;
	}
	/* copy data back to user */
kernel_value.res=v->res;
	if (copy_to_user(&user_value, &kernel_value , sizeof(struct st)))
		return -EFAULT;
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
