#include <linux/module.h>   // Core module macros
#include <linux/kernel.h>   // printk()
#include <linux/init.h>     // __init, __exit
#include <linux/fs.h>       // register_chrdev, file_operations
#include <linux/uaccess.h>  // copy_to_user, copy_from_user
#include<linux/cdev.h>
#include<linux/slab.h>
#include<linux/err.h>
#include<linux/kdev_t.h>


