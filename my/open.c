#include"header.h"
#define mem_size 1024

static char *kernel_buffer;
static dev_t dev=0;
static struct class *dev_class;
static struct cdev my_dev;

static int my_open(struct inode* inode,struct file *file)
{
if(kernel_buffer=kmalloc(mem_size,GFP_KERNEL)){
printk(KERN_INFO "Opened successfully\n");
return 0;
}
else{
printk(KERN_INFO "Unable to open the file\n");
return -1;
}
}
static int my_close(struct inode* inode,struct file *file)
{
kfree(kernel_buffer);
printk(KERN_INFO "Closed successfully\n");
return 0;
}

static ssize_t my_read(struct file *filp,char __user *buf,size_t len,loff_t *off){

copy_to_user(buf,kernel_buffer,mem_size);
pr_info("Data read : success\n");
return mem_size;
}

static ssize_t my_write(struct file*fp, const char __user *buf, size_t len,loff_t *off){

copy_from_user(kernel_buffer,buf,len);
pr_info("Writing : success\n");
return len;
}


static struct file_operations fops={
.owner=THIS_MODULE,
.open=my_open,
.release=my_close,
.read=my_read,
.write=my_write,
.release=my_close,
};

static int __init my_init(void){
int ret;

ret=alloc_chrdev_region(&dev,0,1,"my_file");
pr_info("Major number is %d and Minor number is %d\n",MAJOR(dev),MINOR(dev));

cdev_init(&my_dev,&fops);
ret=cdev_add(&my_dev,dev,1);

//kernel_buffer=kmalloc(mem_size,GFP_KERNEL);
strcpy(kernel_buffer,"hello world");
//cdev_add(&my_dev,dev_no,1);
dev_class=class_create(THIS_MODULE,"my_dev");
IS_ERR(dev_class,NULL,dev_no,NULL,"my_dev");

class_destroy(dev_class);
unregister_chrdev_region(dev,1);
return -1;

printk(KERN_INFO "Loaded successfully\n");
return 0;
}

static void __exit my_exit(void){
kfree(kernel_buffer);
device_destroy(dev_class,dev);
cdev_del(&my_dev);
unregister_chrdev_region(dev,1);
printk(KERN_INFO "Unloaded successfully\n");
}


module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jugal Kishore");
MODULE_DESCRIPTION("Simple read/write operations");
MODULE_VERSION("1.2.3");
