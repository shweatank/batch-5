
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "revstr"
#define BUF_LEN 77

static int major;
static char kernel_buffer[BUF_LEN];
//static int result;

 char str[30];
/* Write from user space */
static ssize_t dev_write(struct file *file, const char __user *buf,
                         size_t len, loff_t *off)
{
    char temp;

    if (copy_from_user(kernel_buffer, buf, len))
        return -EFAULT;

    sscanf(kernel_buffer, "%s", str);

    printk(KERN_INFO "Received: str:%s \n",str);
    for(int i=0,j=4;i<j;i++,j--)
    {
	    pr_info("%c ",str[i]);
	    temp=str[i];
	    str[i]=str[j];
	    str[j]=temp;
    }

    printk(KERN_INFO "After reversing str:%s \n",str);

    return len;
}

/* Read to user space */
static ssize_t dev_read(struct file *file, char __user *buf,
                        size_t len, loff_t *off)
{
    int ret;
    char output[BUF_LEN];

    sprintf(output, "output:%s \n",
            str);

    ret = copy_to_user(buf, output, strlen(output));
    if (ret)
        return -EFAULT;

    return strlen(output);
}

static struct file_operations fops = {
    .read = dev_read,
    .write = dev_write,
};
static int __init basic_char_init(void)
{
    /*
     * Register character device
     * 0 → dynamic major number
     */
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ERR "basic_char: failed to register device\n");
        return major;
    }

    printk(KERN_INFO "basic_char: loaded\n");
    printk(KERN_INFO "basic_char: major number = %d\n", major);
    printk(KERN_INFO "Create device node with:\n");
    printk(KERN_INFO "mknod /dev/%s c %d 0\n", DEVICE_NAME, major);

    return 0;
}

/*
 * Module cleanup
 */
static void __exit basic_char_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "basic_char: unloaded\n");
}

/* Kernel module macros */
module_init(basic_char_init);
module_exit(basic_char_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prashant");
MODULE_DESCRIPTION("Educational basic character driver with file operations");
