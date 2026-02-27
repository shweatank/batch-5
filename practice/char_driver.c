#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>

static int major_number;

ssize_t my_read(struct file *pfile, char __user *buff, size_t buff_size, loff_t *offset)
{
	printk(KERN_INFO"hello from %s\n",__FUNCTION__);
	pr_info("read %d bytes\n",buff_size);
	return 0;
}
ssize_t my_write(struct file *,const char __user *,size_t buff_size, loff_t *offset)
{
	printk(KERN_INFO"hello from %s\n",__FUNCTION__);
	pr_info("wrote %d bytes\n",buff_size);
	return buff_size;
}
int my_open(struct inode *pinode, struct file *pfile)
{
	printk(KERN_INFO"hello from %s\n",__FUNCTION__);
	return 0;
}
int my_release(struct inode *pinode, struct file *pfile)
{
	printk(KERN_INFO"hello from %s\n",__FUNCTION__);
	return 0;
}

struct file_operations fops={
	.owner=THIS_MODULE,
	.open=my_open,
	.release=my_release,
	.read=my_read,
	.write=my_write,
};
static int basic_char_init(void)
{
	printk(KERN_INFO"hello from %s\n",__FUNCTION__);
	/*register with the kernel and indicate that we are registerinf a char dev driver*/
	major_number=register_chrdev(0,"char_driver",&fops);
	if(major_number<0)
	{
		printk(KERN_ERR"basic_char:failed to register device\n");
		return 0;
	}
	pr_info("major number :%d\n",major_number);

	return 0;
}

static void basic_char_exit(void)
{
	pr_info("hello from %s\n",__FUNCTION__);
	unregister_chrdev(major_number,"char_driver"); 
}

module_init(basic_char_init);
module_exit(basic_char_exit); //macros

MODULE_LICENSE("GPL");
MODULE_AUTHOR("prasanna");
MODULE_DESCRIPTION("simple module example");
