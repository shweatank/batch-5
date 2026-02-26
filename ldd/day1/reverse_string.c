#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // copy_to_user, copy_from_user
#include <linux/string.h>

#define DEVICE_NAME "reverse"
#define BUFFER_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ChatGPT");
MODULE_DESCRIPTION("String Reverse Kernel Module");

static int major;
static char kernel_buffer[BUFFER_SIZE];

// Function to reverse a string
void reverse_string(char *str, int len) {
    int i;
    char temp;
    for(i = 0; i < len/2; i++) {
        temp = str[i];
        str[i] = str[len-i-1];
        str[len-i-1] = temp;
    }
}

// Open device
static int dev_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "Device opened\n");
    return 0;
}

// Read from device: send reversed string to user
static ssize_t dev_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset) {
    int bytes_to_copy = strlen(kernel_buffer); // actual string length

    if(len < bytes_to_copy)
        bytes_to_copy = len;

    if(copy_to_user(buffer, kernel_buffer, bytes_to_copy) != 0)
        return -EFAULT;

    return bytes_to_copy;
}

// Write to device: get string from user and reverse it
static ssize_t dev_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) {
    if(len > BUFFER_SIZE-1)
        len = BUFFER_SIZE-1;

    if(copy_from_user(kernel_buffer, buffer, len) != 0)
        return -EFAULT;

    kernel_buffer[len] = '\0'; // null terminate

    // Remove newline if present
    if(kernel_buffer[len-1] == '\n') {
        kernel_buffer[len-1] = '\0';
    }

    reverse_string(kernel_buffer, strlen(kernel_buffer));
    printk(KERN_INFO "Kernel reversed string: %s\n", kernel_buffer);

    return len;
}

// File operations
static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
};

// Module init
static int __init reverse_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if(major < 0) {
        printk(KERN_ALERT "Failed to register device\n");
        return major;
    }
    printk(KERN_INFO "Reverse device registered with major %d\n", major);
    return 0;
}

// Module exit
static void __exit reverse_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Reverse device unregistered\n");
}

module_init(reverse_init);
module_exit(reverse_exit);


