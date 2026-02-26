#include<linux/fs.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/uaccess.h>
#include<linux/kernel.h>

#define DEVICE_NAME "BASIC_CHAR"
static int major_number;



static int basic_open(struct inode *inode,struct file *file)
{
	pr_info("opened success\n");
	return 0;
}

static int basic_close(struct inode *inode,struct file *file)
{
	pr_info("closed success\n");
	return 0;
}

static int basic_read(struct file *file,char __user *user_buffer,size_t count,loff_t *offset)
{
	if(copy_to_user(user_buffer,kernel_buffer,count))
		return -EFAULT;

}
struct file_operations fops=
{
        .owner=THIS_MODULE,
        .open=basic_open,
        .release=basic_close,
	.read=basic_read,
	.write=basic_write,
};


static int __init char_init(void)
{
	major_number=register_chrdev(0,DEVICE_NAME,&fops);
	if(major_number<0)
	{
		pr_info("major number is not allocated\n");
		return major_number;
	}
	pr_info("major is %d\n",major_number);
	pr_info("driver loaded\n");
	return 0;
}
static void __exit char_exit(void)
{
	unregister_chrdev(major_number,DEVICE_NAME);
	pr_info("driver unloaded\n");
}
module_init(char_init);
module_exit(char_exit);

MODULE_LICENSE("GPL");
