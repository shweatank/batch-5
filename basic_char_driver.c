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
#include "common.h"

#define DEVICE_NAME "basic_char"
#define BUF_SIZE    128

static int major_number;
static char kernel_buffer[BUF_SIZE];
static int buffer_size;

static struct result kernel_result;

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
/*static ssize_t basic_read(struct file *file,
                          char __user *user_buffer,
                          size_t count,
                          loff_t *offset)
{
    int bytes_to_copy;

    
     // If offset is beyond data, return 0 (EOF)
     
    if (*offset >= buffer_size)
        return 0;

    bytes_to_copy = min(count, (size_t)(buffer_size - *offset));

    
     // Copy data from kernel space to user space
     
    if (copy_to_user(user_buffer,
                     kernel_buffer + *offset,
                     bytes_to_copy))
        return -EFAULT;

    *offset += bytes_to_copy;

    printk(KERN_INFO "basic_char: read %d bytes\n", bytes_to_copy);
    return bytes_to_copy;
}
*/
/*
 * Called when user writes to /dev/basic_char
 */
/*static ssize_t basic_write(struct file *file,
                           const char __user *user_buffer,
                           size_t count,
                           loff_t *offset)
{
    int bytes_to_copy;

    bytes_to_copy = min(count, (size_t)BUF_SIZE);

    
     // Copy data from user space to kernel space
     
    if (copy_from_user(kernel_buffer,
                       user_buffer,
                       bytes_to_copy))
        return -EFAULT;

    buffer_size = bytes_to_copy;

    printk(KERN_INFO "basic_char: wrote %d bytes\n", bytes_to_copy);
    return bytes_to_copy;
} */

/*static void reverse_string(char *s,int len)
{
	int i,j;
	char temp;
	for(i=0,j=len-1;i<j;i++,j--)
	{
		temp=s[i];
		s[i]=s[j];
		s[j]=temp;
	}
}

static ssize_t basic_write(struct file *file,
                           const char __user *user_buffer,
                           size_t count,
                           loff_t *offset)
{
    int bytes_to_copy;

    bytes_to_copy = min(count, (size_t)(BUF_SIZE-1));

    
     // Copy data from user space to kernel space
     
    if (copy_from_user(kernel_buffer,
                       user_buffer,
                       bytes_to_copy))
        return -EFAULT;

    kernel_buffer[bytes_to_copy]='\0';

    reverse_string(kernel_buffer,bytes_to_copy);

    buffer_size = bytes_to_copy;

    printk(KERN_INFO "basic_char: reversed string: %s\n",kernel_buffer);
    return bytes_to_copy;
}*/





static ssize_t basic_write(struct file *file,
                           const char __user *user_buffer,
                           size_t count,
                           loff_t *offset)
{
	struct data d;
/*	if(count != sizeof(struct data))
		return -EINVAL;*/
	if(copy_from_user(&d,user_buffer,sizeof(d)))
			return -EFAULT;

		switch(d.op)
		{
		case '+':
		kernel_result.res=d.a+d.b;
		break;
		case '-':
		kernel_result.res=d.a-d.b;
		break;
		case '*':
		kernel_result.res=d.a*d.b;
		break;
		case '/':
		kernel_result.res=d.a/d.b;
		break;
		default:
		return -EINVAL;
		}

		buffer_size=sizeof(struct data);
		printk(KERN_INFO "Reult in kernel:%d\n",kernel_result.res);
		return sizeof(struct data);

} 

static ssize_t basic_read(struct file *file,
                          char __user *user_buffer,
                          size_t count,
                          loff_t *offset)
{
    int bytes_to_copy;

    /*
     * If offset is beyond data, return 0 (EOF)
     */
    if (*offset >= sizeof(struct result))
        return 0;

    if(count < sizeof(struct result))
	    return -EINVAL;

    bytes_to_copy = min(count, (size_t)(buffer_size - *offset));

    /*
     * Copy data from kernel space to user space
     */
    if (copy_to_user(user_buffer,
                     &kernel_result + *offset,
                     sizeof(kernel_result.res)))
        return -EFAULT;

    *offset += sizeof(struct result);

   // printk(KERN_INFO "basic_char: read %d bytes\n", bytes_to_copy);
    return sizeof(struct result);
}


/*
 * File operations structure
 * This connects system calls to driver functions
 */
static struct file_operations basic_fops = {       // It is a table of function pointers. It tells the kernel:“If a user performs X operation, call Y function”
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
