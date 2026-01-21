#include <linux/module.h>   // Core module macros
#include <linux/kernel.h>   // printk()
#include <linux/init.h>     // __init, __exit
#include <linux/fs.h>       // register_chrdev, file_operations
#include <linux/uaccess.h>  // copy_to_user, copy_from_user

#define DEVICE_NAME "basic_char"
#define BUF_SIZE    128

static int major_number;
static char kernel_buffer[BUF_SIZE];
static int buffer_size;
char res_buf[10];
static void reverse(char*p)
{
     int l,i,t;
    for(l=0;p[l];l++);
    for(i=0,l=l-1;i<l;i++,l--)
    {
   t=p[i];
     p[i]=p[l];
     p[l]=t; 
    }

}
static void copy(char*d,char*s)
{
while(*d++=*s++);
}
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

/*printk(KERN_INFO "string=%s\n",kernel_buffer);
printk(KERN_INFO "reverse string=%s\n",kernel_buffer);*/
    int n1=0,n2=0,i,res;
     char op;
     for(i=0;kernel_buffer[i];i++)
     {
	     if(kernel_buffer[i]>='0'&&kernel_buffer[i]<='9')
		     n1=n1*10+kernel_buffer[i]-'0';
	     else
	     {
		     op=kernel_buffer[i];      
		     break;
	     }
     }
      for(i=i+1;kernel_buffer[i];i++)
        { 
	     if(kernel_buffer[i]>='0'&&kernel_buffer[i]<='9')
		     n2=n2*10+kernel_buffer[i]-'0';
        }
            printk(KERN_INFO "n1=%d n2=%d op=%c\n",n1,n2,op);
     
	    if(op=='+')     
		    printk(KERN_INFO "%d + %d is %d\n",n1,n2,res=n1+n2);     
	    else  if(op=='-')     
		    printk(KERN_INFO "%d - %d is %d\n",n1,n2,res=n1-n2);     
	    else if(op=='*')     
		    printk(KERN_INFO "%d * %d is %d\n",n1,n2,res=n1*n2);     
	    else  if(op=='/')     
		    printk(KERN_INFO "%d / %d is %d\n",n1,n2,res=n1/n2);     
	    else if(op=='%')     
		    printk(KERN_INFO "%d %% %d is %d\n",n1,n2,res=n1%n2);   
i=0;
while(res)
{
res_buf[i++]=res%10+'0';
res/=10;
}
res_buf[i]='\0';
reverse(res_buf);
printk(KERN_INFO "%s\n",res_buf);
copy(kernel_buffer,res_buf);    
printk(KERN_INFO "kernel=%s\n",kernel_buffer);
bytes_to_copy=i;
printk(KERN_INFO "basic_char: wrote %d bytes\n", bytes_to_copy);
    return bytes_to_copy;
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
