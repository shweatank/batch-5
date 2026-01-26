#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/init.h>

#define DEV_NAME "my_file"
static int major;

static int my_open(struct inode*inode,struct file*file){

pr_info("File is opened\n");
return 0;
}

static int my_close(struct inode*inode,struct file* file){
pr_info("Device closed\n");
return 0;
}

static struct file_operations fops={
.open=my_open,
.release=my_close,
};


static int __init my_init(void){

major=register_chrdev(0,DEV_NAME,&fops);
pr_info("Driver is initialised\n");
pr_info("Major number is %d\n",major);
return 0;
}

static void __exit my_exit(void){
pr_info("device is closed\n");
unregister_chrdev(major,DEV_NAME);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KISHORE THE ALMIGHTY");
MODULE_DESCRIPTION("Basic open and close driver");
