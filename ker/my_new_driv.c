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

#define DEVICE_NAME "open_close"
#define BUF_SIZE    128

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


//  Called when user writes to /dev/basic_char
 
static ssize_t basic_write(struct file *file,
                           const char __user *user_buffer,
                           size_t count,
                           loff_t *offset)
{
    int bytes_to_copy;
    bytes_to_copy = min(count, (size_t)BUF_SIZE);
     //Copy data from user space to kernel space
    if (copy_from_user(kernel_buffer,
                       user_buffer,
                       bytes_to_copy))
        return -EFAULT;
    buffer_size = bytes_to_copy;
    kernel_buffer[buffer_size]='\0';
    int a[5],j=0,i=0,num[3]={0},f=0,k=0;
    char ch;
     while(kernel_buffer[i]!='\0'){
                if(kernel_buffer[i]==','){
                        a[j++]=i;
                }
        i++;
        }
     i=0;
    while(kernel_buffer[i]!= '\0'){
	    if(kernel_buffer[i]==',')
		    i++;
	    if(i-1==a[j-1]){
		     ch=kernel_buffer[i];
		     f++;
	    }else{
	    while(kernel_buffer[i]!=','){
		 num[k]=num[k]*10+(kernel_buffer[i]-'0');
	    		i++;
	    }
	    f++;k++;
	    }
	    if(f==3)
		    break;
    }
    int res=0;
    switch(ch){
	    case '+':res=num[0]+num[1];
		     break;
	    case '-':res=num[0]-num[1];
			break;
	    case '*':res=num[0]*num[1];
			break;
	   case '/':res=num[0]/num[1];
       			break;
	   case '%':res=num[0]%num[1];
			break;
	default : printk("No valid input operator\n");
    }		    
	pr_info("result is %d\n",res);
	memset(kernel_buffer,0,buffer_size);
	i=0;
	int t=res;
	while(t){
		kernel_buffer[i++]=t%10+'0';
		t/=10;
	}
	kernel_buffer[i]='\0';
 char *p,*q,temp;
        p=kernel_buffer;
        //q=kernel_buffer+buffer_size-1;
	q=kernel_buffer+i-1;
        while(p<q){
                temp=*p;
                *p=*q;
                *q=temp;
                p++,q--;
        }
    printk(KERN_INFO "basic_char: wrote %d bytes\n", bytes_to_copy);
    printk(KERN_INFO "kernel_buffer data  is %s\n",kernel_buffer);
    return bytes_to_copy;
}
/*static void basic_reverse(struct file *file,
                           const char __user *user_buffer,
                           size_t count,
                           loff_t *offset)
{
    int bytes_to_copy;

    bytes_to_copy = min(count, (size_t)BUF_SIZE);

    
      Copy data from user space to kernel space
     
    if (copy_from_user(kernel_buffer,
                       user_buffer,
                       bytes_to_copy))
        return -EFAULT;

    buffer_size = bytes_to_copy;
    //char new_buffer[buffer_size];
    //for(i=0;i<buffer_size;i++){
//	    kernel_buffer[i]=new_buffer[
	char *p,*q,temp;
	p=kernel_buffer;
	q=kernel_buffer+buffer_size-1;
	while(p<q){
		temp=*p;
		*p=*q;
		*p=temp;
		p++,q--;
	}
    printk(KERN_INFO "basic_char: wrote %d bytes\n", bytes_to_copy);
    printk(KERN_INFO "reversed data is %s",kernel_buffer); 
    //return bytes_to_copy;
}*/


/*
 * File operations structure
 * This connects system calls to driver functions
 */
static struct file_operations basic_fops = {
    .owner   = THIS_MODULE,
    .open    = basic_open,
    .read    = basic_read,
    .write   = basic_write,
    //.rev     =basic_reverse,
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
