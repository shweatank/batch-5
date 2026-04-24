#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/uaccess.h>
static int major;

static int my_close(struct inode*inode,struct file *file)
{
printk("my close function is called\n");
return 0;
}
static int my_open(struct inode*inode,struct file *file)
{
printk("my_open function called\n");
return 0;
}
static struct file_operations fops={
.open=my_open,
.release=my_close,
//.write=my_write,
//.read=my_read
};

static int __init load_module(void)
{
major=register_chrdev(255,"my_char",&fops);
if(major<0)
{
printk(KERN_INFO"major number not allocated\n");
return 0;
}
pr_info("sudo mknod /dev/%s c %d 0\n","my_char",major);
printk(KERN_INFO"module loaded\n");
return 0;
}
static void __exit unload_module(void)
{
unregister_chrdev(major,"my_char");
pr_info("module unloaded\n");
}

module_init(load_module);
module_exit(unload_module);
MODULE_AUTHOR("SAIKUMAR");
MODULE_DESCRIPTION("char driver");
MODULE_LICENSE("GPL");
