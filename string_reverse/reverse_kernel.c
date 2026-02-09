#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "reverse_dev"
#define MAJOR_NUM 511

static char kbuf[100];   // kernel buffer for reversed string
static int buf_len = 0;  // length of reversed string
static int read_done = 0; // track if read has already been done

static int reverse_open(struct inode *inode, struct file *file) {
    read_done = 0; // reset read flag on open
    return 0;
}

// write: store and reverse string
static ssize_t reverse_write(struct file *file, const char __user *buf, size_t len, loff_t *offset) {
    int i;

    if (len >= sizeof(kbuf))
        len = sizeof(kbuf) - 1;

    if (copy_from_user(kbuf, buf, len))
        return -EFAULT;

    kbuf[len] = '\0'; // null-terminate

    // reverse string in place
    for (i = 0; i < len / 2; i++) {
        char tmp = kbuf[i];
        kbuf[i] = kbuf[len - i - 1];
        kbuf[len - i - 1] = tmp;
    }

    buf_len = len;
    return len;
}

// read: return reversed string (only once)
static ssize_t reverse_read(struct file *file, char __user *buf, size_t len, loff_t *offset) {
    if (read_done)  // EOF after first read
        return 0;

    if (len > buf_len)
        len = buf_len;

    if (copy_to_user(buf, kbuf, len))
        return -EFAULT;

    read_done = 1;
    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = reverse_read,
    .write = reverse_write,
    .open = reverse_open,
};

static int __init reverse_init(void) {
    int ret = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);
    if (ret < 0) {
        printk(KERN_ALERT "Failed to register device\n");
        return ret;
    }
    printk(KERN_INFO "Reverse device registered with major %d\n", MAJOR_NUM);
    return 0;
}

static void __exit reverse_exit(void) {
    unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
    printk(KERN_INFO "Reverse device unregistered\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mirafra");
MODULE_DESCRIPTION("String Reversal Character Device");

module_init(reverse_init);
module_exit(reverse_exit);

