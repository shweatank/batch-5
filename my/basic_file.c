#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/uaccess.h>
#define DEV_NAME "/dev/my_file"
#define BUFF_SIZE 1024

static int major;
static char k_buf[BUFF_SIZE];
static int data_size=0;

static ssize_t my_write(struct file*file, const char __user *usr,size_t len, loff_t *offset)
{

	if(len>BUFF_SIZE)
		len=BUFF_SIZE;

	if(copy_from_user(k_buf,usr,len))
		return -EFAULT;

	char *p=k_buf,*q=k_buf+len-1,temp;
	while(p<q){
		temp=*p;
		*p=*q;
		*q=temp;
		p++,q--;
	}

	pr_info("Reversed data is %s\n",k_buf);

	data_size=len;
	pr_info("Data is successfully read\n");
	*offset=0;
	return len;
}


static ssize_t my_read(struct file*file, char __user *usr,size_t len, loff_t *offset)
{

	if(*offset>=data_size)
		return 0;

	if(len>data_size-*offset){
		len=data_size-*offset;
	}

	if(copy_to_user(usr,k_buf+*offset,len))
		return -EFAULT;

	*offset=*offset+len;
	pr_info("Data is wrote successfully\n");
	return len;

}

static int my_open(struct inode*inode,struct file*file){

	pr_info("File is opened successfully\n");
	return 0;
}

static int my_close(struct inode*inode,struct file*file){

	pr_info("File is closed successfully\n");
	return 0;
}

static struct file_operations fops={
	.open=my_open,
	.release=my_close,
	.read=my_read,
	.write=my_write,
	.owner=THIS_MODULE,
};

static int __init my_init(void){

	major=register_chrdev(0,DEV_NAME,&fops);
	pr_info("Major number is %d\n",major);
	return 0;
}
static void __exit my_exit(void){

	unregister_chrdev(major,DEV_NAME);
	pr_info("Exited from driver\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("KOSHORE");
MODULE_DESCRIPTION("Simple file operations driver");
