#include<linux/module.h>
#include<linux/init.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/kernel.h>
#include<linux/err.h>

dev_t dev=0;

static void my_exit(void);

static struct class *dev_class;

static int __init  my_init(void)
{

	// allocation of major number
	if(alloc_chrdev_region(&dev,0,1,"my_device")<0)
	{
		pr_info("major number allocation failed:\n");
		return -1;
	}

	pr_info("major=%d minor= %d\n",MAJOR(dev),MINOR(dev));

	//creation of sysfs entry in order to provide to  udev event

	        dev_class = class_create("etx_class");
        if(IS_ERR(dev_class)){
            pr_err("Cannot create the struct class for device\n");
            goto r_class;
        }

	pr_info("device class is created successfully:\n");

	if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"my_device")))
	{
		pr_info("failed to device create  :\n");

		goto r_device;
	}

	pr_info("module is inserted success fully:\n");

	return 0;


r_class:class_destroy(dev_class);
r_device:unregister_chrdev_region(dev,1);


	 return -1;

}
static void __exit my_exit(void)
{

       unregister_chrdev_region(dev,1);
       class_destroy(dev_class);
       device_destroy(dev_class,dev);
       pr_info("module removed success fully:\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("gurunath@gmail.com");
MODULE_DESCRIPTION("Simple linux driver (Automatically Creating a Device file)");
