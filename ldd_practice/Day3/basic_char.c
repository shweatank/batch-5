#include<linux/module.h>
#include<linux/init.h>
#include<linux/uaccess.h>
#include<linux/kernel.h>
#include<linux/fs.h>

#define DEVICE_NAME "basic_driver"
#define BUF_SIZE 128

static int major;
static char kernel_buf[BUF_SIZE];
static int buffer_size;

static size_t basic_open(struct inode *inode,struct file *file){
	printk("basic_driver opened in user_space\n");
	return 0;
}

static size_t basic_close(struct inode *inode,struct file *file){
	printk("basic_driver closed in user_space\n");
	return 0;
}

static size_t basic_read(struct file *file,char __user *user_buf,size_t count,loff_t *offset){
	int bytes_to_copy;

	if(*offset>=bufer_size)
		return 0;
	bytes_to_copy=min(count,(size_t)(buffer_size-*offset));

	if(copy_to_user(user_buffer,kernel_buffer+*offset,bytes_to_copy))
		return -EFAULT;
	*offset+=bytes_to_copy;
	printk("basic_char:read %d bytes\n");
	return bytes_to_copy;
}

static size_t basic_write(struct file *file,const char __user *user_buf,size_t count,loff_t *offset){
	int bytes_to_copy;
	bytes_to_copy=min(count,(size_t)BUF_SIZE);

	if(copy_from_user(kernel_buf,user_buf,bytes_to_copy))
		return -EFAULT;
	buffer_size=bytes_to_copy;
	printk("basic_char: wrote %d bytes\n",bytes_to_copy);
	return bytes_to_copy;
}


static struct file_operations fops={
	.owner=THIS_MODULE,
	.open=basic_open,
	.read=basic_read,
	.write=basic_write,
	.release=basic_close,
	.unlocked_ioctl=basic_ioctl,	
};


static int __init basic_init(void){
	major=register_chrdev(0,DEVICE_NAME,&fops);
	
	if(major<0){
		printk("device register failed\n");
		return major;
	}
	printk("basic_driver loaded\n");
	printk("basic_driver major=%d\n",major);
	printk("create node for the driver use\n");
	printk("sudo chmod 666 /dev/%s c %d 0\n",DEVICE_NAME,major);
	return 0;
}

static void __exit basic_exit(void){
	unregister_chedev(major,DEVICE_NAME);
	printk("Basic Driver Unloaded\n");
}



module_init(basic_init);
module_exit(basic_exit);

MODULE_LICENSE("GPL");


