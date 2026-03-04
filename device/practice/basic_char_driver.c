#include<linux/fs.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/uaccess.h>
#include<linux/kernel.h>

#define DEVICE_NAME "BASIC_CHAR"
#define BUFFER 150

static char kernel_buffer[BUFFER];
static int major_number;
static int buffer_size;



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

static ssize_t basic_write(struct file *file ,const char __user *user_buffer,size_t count, loff_t *offset)
{
	int bytes_to_copy;
	bytes_to_copy=min(count,(size_t)BUFFER);
	if(copy_from_user(kernel_buffer,user_buffer,bytes_to_copy))
		return -EFAULT;
	buffer_size = bytes_to_copy;
	printk("write success wrote %d\n",bytes_to_copy);
	printk("wrote into buffer %s\n",kernel_buffer);
	return bytes_to_copy;
}

static ssize_t basic_read(struct file *file,char __user *user_buffer,size_t count,loff_t *offset)
{
	int bytes_to_copy;
	if(*offset>=buffer_size)
		return 0;
	bytes_to_copy=min(count,(size_t)(buffer_size-*offset));
	if(copy_to_user(user_buffer,kernel_buffer,bytes_to_copy))
		return -EFAULT;
	*offset+=bytes_to_copy;
	printk("read %d bytes\n",bytes_to_copy);
	printk("wrote %s\n",kernel_buffer);
	return bytes_to_copy;

}
struct file_operations fops=
{
        .owner=THIS_MODULE,
        .open=basic_open,
	.read=basic_read,
	.write=basic_write,
        .release=basic_close,
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
