#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/uaccess.h>

#define DEVICE_NAME "basic_char"
static int major_num;

static int basic_open(struct inode *inode,struct file *file)
{
	printk("basic_char : device opened\n");
	return 0;
}

static int basic_close(struct inode *inode,struct file *file)
{
	printk("basic_char :device closed\n");
	return 0;
}
static struct file_operations basic_fops={
	.owner=THIS_MODULE,
	.open= basic_open,
	.read=basic_read,
	.write=basic_write,
	.release=basic_close,
};
static int __init basic_char_init(void)
{
        major_num=register_chrdev(0,DEVICE_NAME,&basic_fops);
        if(major_num<0)
        {
                pr_err("failed to reg char driver\n");
                return major_num;
        }
        pr_info("driver loaded\n");
        pr_info("major_num : %d\n",major_num);
        return 0;
}

static void __exit basic_char_exit(void)
{
        unregister_chrdev(major_num,DEVICE_NAME);
        pr_info("basic_char unloaded\n");
}

module_init(basic_char_init);
module_exit(basic_char_exit);


MODULE_LICENSE("GPL");
