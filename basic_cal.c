/*
 * basic_char_driver.c
 *
 * Educational Linux character device driver.
 * Demonstrates:
 *  - module init / exit
 *  - character device registration
 *  - basic file operations (open, read, write, release)
 *
 * No real hardware is accessed.
 */

#include <linux/module.h>   // Core module macros
#include <linux/kernel.h>   // printk()
#include <linux/init.h>     // __init, __exit
#include <linux/fs.h>       // register_chrdev, file_operations
#include <linux/uaccess.h>  // copy_to_user, copy_from_user

#define DEVICE_NAME "basic_char"
#define BUF_SIZE    128
int operation(int,int,char);
static int major_number;
static char kernel_buffer[BUF_SIZE];
static int buffer_size;

/*
 * Called when user opens /dev/basic_char
 */
static int basic_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "basic_char: device opened\n");
    return 0;
}

/*
 * Called when user closes /dev/basic_char
 */
static int basic_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "basic_char: device closed\n");
    return 0;
}

/*
 * Called when user reads from /dev/basic_char
 */
static ssize_t basic_read(struct file *file,
		char __user *user_buffer,
		size_t count,
		loff_t *offset)
{
	int bytes_to_copy;

	/*
	 * If offset is beyond data, return 0 (EOF)
	 */
	if (*offset >= buffer_size)
		return 0;

	bytes_to_copy = min(count, (size_t)(buffer_size - *offset));

	/*
	 * Copy data from kernel space to user space
	 */
	if (copy_to_user(user_buffer,
				kernel_buffer + *offset,
				bytes_to_copy))
		return -EFAULT;

	*offset += bytes_to_copy;

	printk(KERN_INFO "basic_char: read %d bytes\n", bytes_to_copy);
	return bytes_to_copy;
}

/*
 * Called when user writes to /dev/basic_char
 */


static ssize_t basic_write(struct file *file,
		const char __user *user_buffer,
		size_t count,
		loff_t *offset)
{
	int bytes_to_copy;

	bytes_to_copy = min(count, (size_t)BUF_SIZE);

	/*
	 * Copy data from user space to kernel space
	 */
	if (copy_from_user(kernel_buffer,
				user_buffer,
				bytes_to_copy))
		return -EFAULT;

	buffer_size = bytes_to_copy;

	printk(KERN_INFO "basic_char: wrote %d bytes\n", bytes_to_copy);
	int num1=0,num2=0,flag=0,rev=0,temp,i;
	char ch,*p;
	for(i=0;kernel_buffer[i];i++)
	{
		if(kernel_buffer[i]>='0' && kernel_buffer[i]<='9' )
			continue;
		else if (kernel_buffer[i]=='+' || kernel_buffer[i]=='-' || kernel_buffer[i]=='*' || kernel_buffer[i]=='/')
			continue;
		else
			break;
	}
	kernel_buffer[i]='\0';
	for(int a=0,b=i-1;a<b;a++,b--){
		char temp=kernel_buffer[a];
		kernel_buffer[a]=kernel_buffer[b];
		kernel_buffer[b]=temp;
	}
p=kernel_buffer;

	while(*p){

		if(*p>='0' && *p<='9'){
			if(flag==0)
				num1=num1*10+(*p-48);
			else
				num2=num2*10+(*p-48);
		}
		else if (*p=='+' || *p=='-' || *p=='*' || *p=='/'){
			flag=1;
			ch=*p;
		}
		else
			break;
		p++;
	}
	char q[10];
	int res=operation(num1,num2,ch);
	for(temp=res;temp;temp=temp/10,i++){
		q[i]=temp%10+48;
	}
	q[i]='\0';
	for(int a=0,b=i-1;a<b;a++,b--){
		char temp=q[a];
		q[a]=q[b];
		q[b]=temp;
	}
	for(i=0;q[i];i++)
		kernel_buffer[i]=q[i];
	kernel_buffer[i]='\0';
	num1=0,num2=0,flag=0,rev=0,temp=0,i=0;
//bytes_to_copy=i;

	return bytes_to_copy;
}

int operation(int a,int b,char ch){
	switch (ch){
		case '+': return a+b;
		case '-':return a-b;
		case '*':return a*b;
		case '/':return a/b;
	}
return 0;
}

/*
 * File operations structure
 * This connects system calls to driver functions
 */
static struct file_operations basic_fops = {
	.owner   = THIS_MODULE,
	.open    = basic_open,
	.read    = basic_read,
	.write   = basic_write,
    .release = basic_release,
};

/*
 * Module initialization
 */
static int __init basic_char_init(void)
{
    /*
     * Register character device
     * 0 → dynamic major number
     */
    major_number = register_chrdev(0, DEVICE_NAME, &basic_fops);
    if (major_number < 0) {
        printk(KERN_ERR "basic_char: failed to register device\n");
        return major_number;
    }

    printk(KERN_INFO "basic_char: loaded\n");
    printk(KERN_INFO "basic_char: major number = %d\n", major_number);
    printk(KERN_INFO "Create device node with:\n");
    printk(KERN_INFO "mknod /dev/%s c %d 0\n", DEVICE_NAME, major_number);

    return 0;
}

/*
 * Module cleanup
 */
static void __exit basic_char_exit(void)
{
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "basic_char: unloaded\n");
}

/* Kernel module macros */
module_init(basic_char_init);
module_exit(basic_char_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prashant");
MODULE_DESCRIPTION("Educational basic character driver with file operations");
