#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>

#define DEVICE_NAME "reverse_dev"
#define BUF_SIZE 256

static dev_t dev_num;
static struct cdev rev_cdev;

static char kernel_buf[BUF_SIZE];
static int data_len;

/* ================================
 * Utility: Reverse string in-place
 * ================================ */
static void reverse_string(char *str, int len)
{
    int i;
    char temp;

    for (i = 0; i < len / 2; i++) {
        temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

/* ================================
 * File operations
 * ================================ */
static int rev_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "reverse_dev: opened\n");
    return 0;
}

static int rev_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "reverse_dev: closed\n");
    return 0;
}

static ssize_t rev_write(struct file *file,
                         const char __user *user_buf,
                         size_t count,
                         loff_t *offset)
{
    if (count >= BUF_SIZE)
        count = BUF_SIZE - 1;

    if (copy_from_user(kernel_buf, user_buf, count))
        return -EFAULT;

    kernel_buf[count] = '\0';
    data_len = count;

    reverse_string(kernel_buf, data_len);

    printk(KERN_INFO "reverse_dev: received & reversed string\n");
    return count;
}

static ssize_t rev_read(struct file *file,
                        char __user *user_buf,
                        size_t count,
                        loff_t *offset)
{
    if (*offset >= data_len)
        return 0;  // EOF

    if (count > data_len - *offset)
        count = data_len - *offset;

    if (copy_to_user(user_buf,
                     kernel_buf + *offset,
                     count))
        return -EFAULT;

    *offset += count;
    return count;
}

/* ================================
 * File operations table
 * ================================ */
static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = rev_open,
    .write   = rev_write,
    .read    = rev_read,
    .release = rev_release,
};

/* ================================
 * Module init / exit
 * ================================ */
static int __init rev_init(void)
{
    alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);

    cdev_init(&rev_cdev, &fops);
    cdev_add(&rev_cdev, dev_num, 1);

    printk(KERN_INFO "reverse_dev loaded: Major=%d Minor=%d\n",
           MAJOR(dev_num), MINOR(dev_num));
    return 0;
}

static void __exit rev_exit(void)
{
    cdev_del(&rev_cdev);
    unregister_chrdev_region(dev_num, 1);
    printk(KERN_INFO "reverse_dev unloaded\n");
}

module_init(rev_init);
module_exit(rev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Character Driver: Reverse String from User Space");
