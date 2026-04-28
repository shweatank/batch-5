#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "addsub"
#define BUF_LEN 80

static int major;
static char kernel_buffer[BUF_LEN];
static int result;

/* Write from user space */
static ssize_t dev_write(struct file *file, const char __user *buf,
                         size_t len, loff_t *off)
{
    int a, b;
    char c;

    if (copy_from_user(kernel_buffer, buf, len))
        return -EFAULT;

    sscanf(kernel_buffer, "%d%c%d", &a,&c,&b);

    switch(c)
    {
	    case '+':
    			result = a + b;
			break;
	    case '-':
  			  result = a - b;
			  break;
	    case '*':
			  result =a * b;
		      break;
	    case '/':
		      result =a / b;
		      break;

	    case '%': result =a * b;
		      break;
    }

    printk(KERN_INFO "Received: a=%d b=%d\n", a, b);

    return len;
}

/* Read to user space */
static ssize_t dev_read(struct file *file, char __user *buf,
                        size_t len, loff_t *off)
{
    int ret;
    char output[BUF_LEN];

    sprintf(output, "output:%d",
            result);

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
