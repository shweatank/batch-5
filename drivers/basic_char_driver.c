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

static int major_number;
static char kernel_buffer[BUF_SIZE];
static int buffer_size;


void reverse(char *str) {
        int i = 0,j = strlen(str)-1;
        for(;i<j;i++,j--) {
                char temp = str[i];
                str[i] = str[j];
                str[j] = temp;
        }
}
/*
void operation(char *str) {
	
                int num1 = 0,num2 = 0,count = 0,res;
                while((str[count] >= '0' && str[count] <= '9')) {
                        num1 = num1*10 + str[count]-'0';
                        count++;
                }
                count++;
                while((str[count] >= '0' && str[count] <= '9')) {
                        num2 = num2*10 + str[count]-'0';
                        count++;
                }
                count++;
                switch(str[count]) {
                        case '+':
                                res = num1+num2;
                                break;
                        case '-':
                                res = num1-num2;
                                break;
                        case '*':
                                res = num1*num2;
                                break;
                        case '/':
                                res = num1/num2;
                                break;
                        default :
                                break;
                }
	   count = 0;
	   while(res) {
                str[count] = res % 10;
                count++;
                res /= 10;
	}
	
       // str[count] = '\0';
        //reverse(str);
	
}
*/

void operation(char *str)
{
    int num1 = 0, num2 = 0, res = 0, i = 0, sign = 0;
    char op;

    while (str[i] >= '0' && str[i] <= '9') {
        num1 = num1 * 10 + (str[i] - '0');
        i++;
    }

    op = str[i];
    i++;

    while (str[i] >= '0' && str[i] <= '9') {
        num2 = num2 * 10 + (str[i] - '0');
        i++;
    }

    switch (op) {
        case '+': res = num1 + num2; break;
        case '-': res = num1 - num2; break;
        case '*': res = num1 * num2; break;
        case '/': res = (num2 != 0) ? num1 / num2 : 0; break;
        default:  return;
    }

    i = 0;
    if (res < 0) {
        sign = 1;
        res = -res;
    }

    do {
        str[i++] = (res % 10) + '0';
        res /= 10;
    } while (res);

    if (sign)
        str[i++] = '-';

    str[i] = '\0';

    reverse(str);

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

    //reverse(kernel_buffer);
   operation(kernel_buffer);

    if (copy_to_user(user_buffer,
                     kernel_buffer + *offset,
                     bytes_to_copy))
        return -EFAULT;

    *offset += bytes_to_copy;

    printk(KERN_INFO "basic_char: read %s bytes\n", kernel_buffer);
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
