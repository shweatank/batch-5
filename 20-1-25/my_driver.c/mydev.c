#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/uaccess.h>

#define DEVICE_NAME "mydev"

int major;
int k_var;


struct file_operations fops=
{
	.write=my_write,
};

ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *pos)
{
	copy_from_user(&k_var,buf,sizeof(int));
	printk(KERN_INFO "integer received from user space: %d\n",k_var);

	return sizeof(int);
}
static int __init my_init(void)
{
	major=register_chrdev(0,DEVICE_NAME,&fops);
	printk(KERN_INFO:"driver loaded major no=%d\n",major);
	return 0;
}
static int __exit my_exit(void)
{
	unregister_chrdev(major,DEVICE_NAME);
	printk(KERN_INFO:"driver loaded major no=%d\n",major);
	return 0;
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
