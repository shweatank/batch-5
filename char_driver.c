#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/fs.h>
static int major;
#define DEVICE_NAME "char_file"


static int my_open(struct inode *inode, struct file *file)
{
    pr_info("%s: device opened\n",DEVICE_NAME);
    return 0;
}

/* ---------- release ---------- */
static int my_close(struct inode *inode, struct file *file)
{
    pr_info("%s: device closed\n",DEVICE_NAME);
    return 0;
}
static struct file_operations fops={
   .owner=THIS_MODULE,
   .open=my_open,
   .release=my_close
};

static int __init load_init(void)
{
pr_info("driver loaded\n");
major=register_chrdev(0,DEVICE_NAME,&fops);
if(major<0)
{
pr_err("major number not allocated for %s driver\n ",DEVICE_NAME);
return major;
}
printk(KERN_INFO "DEVICE NAME: %s, major number=%d\n",DEVICE_NAME,major);
printk(KERN_INFO "type - sudo mknod /dev/%s c %d 0\n",DEVICE_NAME,major);
return 0;
}

static void __exit unload_exit(void)
{
unregister_chrdev(major,DEVICE_NAME);
pr_info("driver unloaded\n");
}
module_init(load_init);
module_exit(unload_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SAI KUMAR");
MODULE_DESCRIPTION("character driver demo program");
