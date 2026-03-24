// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define DEVICE_NAME "mychardev"
#define CLASS_NAME  "mycharclass"
#define BUF_SIZE    256

struct mychardev_data {
	struct cdev cdev;
	struct class *class;
	struct device *device;
	dev_t devt;
	char *buffer;
	struct mutex lock;
};

static struct mychardev_data *mydev;

static int mychardev_open(struct inode *inode, struct file *file)
{
	file->private_data = mydev;
	return 0;
}

static int mychardev_release(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t mychardev_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	struct mychardev_data *data = file->private_data;
	ssize_t ret;

	if (mutex_lock_interruptible(&data->lock))
		return -ERESTARTSYS;
	if (*ppos >= BUF_SIZE) {
		ret = 0;
		goto out;
	}
	if (count > BUF_SIZE - *ppos)
		count = BUF_SIZE - *ppos;
	if (copy_to_user(buf, data->buffer + *ppos, count)) {
		ret = -EFAULT;
		goto out;
	}
	*ppos += count;
	ret = count;

out:
	mutex_unlock(&data->lock);
	return ret;
}

static ssize_t mychardev_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	struct mychardev_data *data = file->private_data;
	ssize_t ret;

	if (mutex_lock_interruptible(&data->lock))
		return -ERESTARTSYS;
	if (*ppos >= BUF_SIZE) {
		ret = -ENOSPC;
		goto out;
	}
	if (count > BUF_SIZE - *ppos)
		count = BUF_SIZE - *ppos;
	if (copy_from_user(data->buffer + *ppos, buf, count)) {
		ret = -EFAULT;
		goto out;
	}
	*ppos += count;
	ret = count;

out:
	mutex_unlock(&data->lock);
	return ret;
}

static const struct file_operations mychardev_fops = {
	.owner = THIS_MODULE,
	.open = mychardev_open,
	.release = mychardev_release,
	.read = mychardev_read,
	.write = mychardev_write,
};

static int __init mychardev_init(void)
{
	int ret;

	mydev = kzalloc(sizeof(*mydev), GFP_KERNEL);
	if (!mydev)
		return -ENOMEM;

	ret = alloc_chrdev_region(&mydev->devt, 0, 1, DEVICE_NAME);
	if (ret)
		goto err_alloc_chrdev;

	cdev_init(&mydev->cdev, &mychardev_fops);
	mydev->cdev.owner = THIS_MODULE;
	ret = cdev_add(&mydev->cdev, mydev->devt, 1);
	if (ret)
		goto err_cdev_add;

	mydev->class = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(mydev->class)) {
		ret = PTR_ERR(mydev->class);
		goto err_class_create;
	}

	mydev->device = device_create(mydev->class, NULL, mydev->devt, NULL, DEVICE_NAME);
	if (IS_ERR(mydev->device)) {
		ret = PTR_ERR(mydev->device);
		goto err_device_create;
	}

	mydev->buffer = kzalloc(BUF_SIZE, GFP_KERNEL);
	if (!mydev->buffer) {
		ret = -ENOMEM;
		goto err_buffer_alloc;
	}

	mutex_init(&mydev->lock);
	pr_info("mychardev: loaded\n");
	return 0;

err_buffer_alloc:
	device_destroy(mydev->class, mydev->devt);
err_device_create:
	class_destroy(mydev->class);
err_class_create:
	cdev_del(&mydev->cdev);
err_cdev_add:
	unregister_chrdev_region(mydev->devt, 1);
err_alloc_chrdev:
	kfree(mydev);
	return ret;
}

static void __exit mychardev_exit(void)
{
	if (mydev) {
		kfree(mydev->buffer);
		device_destroy(mydev->class, mydev->devt);
		class_destroy(mydev->class);
		cdev_del(&mydev->cdev);
		unregister_chrdev_region(mydev->devt, 1);
		kfree(mydev);
	}
	pr_info("mychardev: unloaded\n");
}

module_init(mychardev_init);
module_exit(mychardev_exit);

MODULE_AUTHOR("Your Name <your@email.com>");
MODULE_DESCRIPTION("Simple character device driver (modern, error-safe)");
MODULE_LICENSE("GPL v2");
