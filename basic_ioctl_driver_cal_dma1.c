// basic_ioctl_drv.c
// Minimal IOCTL example: send an int from user → kernel → modify → return

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include<linux/slab.h> //kmalloc
#include<linux/vmalloc.h> //vmalloc

#define DEVICE_NAME "basic_ioctl"
#define IOCTL_MAGIC 'B'
#define KMALLOC_SIZE 128
#define VMALLOC_SIZE (8 * PAGE_SIZE)
struct st
{
	int a;
	int b;
	int res;
};
//#define IOCTL_SET_VALUE _IOWR(IOCTL_MAGIC, 1, int)
#define ADD _IOWR(IOCTL_MAGIC, 1, struct st)
#define SUB _IOWR(IOCTL_MAGIC, 2,struct st)
#define MUL _IOWR(IOCTL_MAGIC, 3, struct st)
#define DIV _IOWR(IOCTL_MAGIC, 4, struct st)

static int major;
static struct st *kernel_value;

/* ioctl handler */
static long basic_ioctl(struct file *file,
                        unsigned int cmd,
                        unsigned long arg)
{
	struct st __user *user_data = (struct st __user *)arg;
    //struct st kernel_value;

	/* copy data from user */	
    if (copy_from_user(kernel_value,user_data, sizeof(struct st)))
            return -EFAULT;

    switch (cmd) {
    case ADD:
        pr_info("Kernel: received %d %d from user\n", kernel_value->a,kernel_value->b);
	
        /* kernel modifies data */
        kernel_value->res=kernel_value->a + kernel_value->b;

        break;
     case SUB:
        pr_info("Kernel: received %d %d from user\n", kernel_value->a,kernel_value->b);
        kernel_value->res= kernel_value->a - kernel_value->b;


        break;
      case MUL:

        pr_info("Kernel: received %d %d from user\n", kernel_value->a,kernel_value->b);
        kernel_value->res= kernel_value->a * kernel_value->b;

        break;
      case DIV:
        pr_info("Kernel: received %d %d from user\n", kernel_value->a,kernel_value->b);
	if (kernel_value->b == 0)
            return -EINVAL;
        kernel_value->res= kernel_value->a / kernel_value->b;


        break;


    default:pr_info("returning..\n");
        return -EINVAL;
    }
    pr_info("result:%d\n",kernel_value->res);
     if (copy_to_user(user_data,kernel_value, sizeof(struct st)))
            return -EFAULT;
    return 0;
}

static struct file_operations fops = {
    .owner          = THIS_MODULE,
    .unlocked_ioctl = basic_ioctl,
};

static int __init basic_init(void)
{
	int *seg,a=5,b=0;
	kernel_value=(struct st *)kmalloc(sizeof(struct st),GFP_ATOMIC);
	//pr_info("seg_fault:%d\n",(*seg)); //...>gives killed since it is segfault
	//pr_info("div by zero:%d\n",a/b);  ///...>insmod doesn't work due to divide by zero & you be waiting there like while(1)
	for(;;){a/b;	}	//crashing system
	major = register_chrdev(0, DEVICE_NAME, &fops);
    pr_info("basic_ioctl loaded, major=%d\n", major);
    return 0;
}

static void __exit basic_exit(void)
{
	kfree(kernel_value);
    unregister_chrdev(major, DEVICE_NAME);
    pr_info("basic_ioctl unloaded\n");
}

module_init(basic_init);
module_exit(basic_exit);

MODULE_LICENSE("GPL");
