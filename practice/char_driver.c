#include"kheader.h"

#define DEVICE_NAME "char_driver"
#define bufsize 120
static int major;
static char k_buf[bufsize];

static int my_open(struct inode *inode, struct file *file)
{
    pr_info("%s: device opened\n",DEVICE_NAME);
    return 0;
}

static int my_close(struct inode *inode, struct file *file)
{
    pr_info("%s: device closed\n",DEVICE_NAME);
    return 0;
}
static ssize_t my_write(struct file *file,const char __user *u_buf,size_t count,loff_t *offset)
{
if(count>bufsize)
return 0;
if(copy_from_user(k_buf,u_buf,count))
return -EFAULT;
pr_info("write %lu bytes\n",count);
pr_info("kbuf=%s\n",k_buf);
return count;
}
static ssize_t my_read(struct file *file,char __user *u_buf,size_t count,loff_t *offset)
{
int len=strlen(k_buf);
if(*offset>=len)
return 0;

if(count>len-*offset)
count=len-*offset;

if(copy_to_user(u_buf,k_buf+*offset,count))
return -EFAULT;

*offset+=count;
pr_info("read %lu bytes\n",count);
return count;
}
static struct file_operations fops={
   .owner=THIS_MODULE,
   .open=my_open,
   .release=my_close,
   .write=my_write,
   .read=my_read
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
