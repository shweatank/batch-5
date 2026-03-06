/* basic structure for any kernel module*/

#include<linux/module.h>     //required for loadable modules
#include<linux/kernel.h>     //required for api like printk()
#include<linux/init.h>       //for macros init and exit
#include<linux/fs.h>         //for filesys and register the char driver
#include<linux/uaccess.h>    //copy_to_user() copy_from_user

#define DEVICE_NAME "basic_char"
#define BUF_SIZE 128

static int major_nu;
static char kernel_buffer[BUF_SIZE];
static int buffer_size;

/*called when user open /dev/basic_char*/
static int basic_open(struct inode *inode,struct file *file)
{
printk(KERN_INFO"File opened\n");
return 0;
}

/*called when user close /dev/basic_char*/
static int basic_release(struct inode *inode,struct file *file)
{
printk(KERN_INFO"File closed\n");
return 0;
}

static ssize_t basic_read(struct file *file,char __user *user_buffer,size_t count,loff_t *offset)
{
printk(KERN_INFO"File read\n");
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

static ssize_t basic_write(struct file *file,const char __user *user_buffer,size_t count,loff_t *offset)
{
printk(KERN_INFO"File write\n");
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
    return bytes_to_copy;
}

static struct file_operations basic_fops = {
    .owner   = THIS_MODULE,
    .open    = basic_open,
    .read    = basic_read,
    .write   = basic_write,
    .release = basic_release,
};

/*init function is executed when module is loaded*/
static int __init init_fun(void)
{
major_nu=register_chrdev(0,DEVICE_NAME,&basic_fops);
if(major_nu<0)
{
printk(KERN_ERR"failed to create char device\n");
return major_nu;
}
printk(KERN_INFO"module loaded\n");
pr_info("%d is major number\n",major_nu);
printk(KERN_INFO"mknod /dev/%s c %d 0\n",DEVICE_NAME,major_nu);
return 0;
}

/*exit function is executed when module is unloaded*/
static void __exit exit_fun(void)
{
unregister_chrdev(major_nu,DEVICE_NAME);
printk(KERN_INFO"module unloaded\n");
}

/*macros for init and exit*/
module_init(init_fun);
module_exit(exit_fun);


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("BASIC char driver");
MODULE_AUTHOR("VARSHITHA");
MODULE_VERSION("2:1.0");

