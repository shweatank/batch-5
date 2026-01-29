#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "proc_driver"
#define BUF_LEN   128

static char proc_buffer[BUF_LEN];
static size_t proc_buffer_len;
struct d
{
	int d1,d2;
	char ch;
};
static struct d data;
static int result;

static ssize_t proc_read(struct file *file,
                         char __user *user_buf,
                         size_t count,
                         loff_t *ppos)
{
    return simple_read_from_buffer(user_buf,
                                   count,
                                   ppos,
                                   proc_buffer,
                                  proc_buffer_len);


//	if(copy_to_user(user_buf,&result,4))
//		return -EFAULT;
//	return;
}

static ssize_t proc_write(struct file *file,
                          const char __user *user_buf,
                          size_t count,
                          loff_t *ppos)
{
    if (count > BUF_LEN - 1)
        count = BUF_LEN - 1;

    if (copy_from_user(&data, user_buf, count))
        return -EFAULT;

   // proc_buffer[count] = '\0';
   // proc_buffer_len = count;

    //pr_info("proc_basic: received \"%s\"\n", proc_buffer);
    switch(data.ch)
    {
	    case 'A':result=data.d1+data.d2;
		     break;
	    case 'S':result=data.d1-data.d2;
		     break;
	    case 'M':result=data.d1*data.d2;
		     break;
	    case 'D':if(data.d2!=0)
		     	result=data.d1/data.d2;
		     break;
    }
    pr_info("result : %d\n",result);

    return count;
}

static const struct proc_ops proc_fops = {
    .proc_read  = proc_read,
    .proc_write = proc_write,
};

static int __init proc_basic_init(void)
{
    proc_create(PROC_NAME, 0666, NULL, &proc_fops);
    pr_info("proc_basic: loaded\n");
    return 0;
}

static void __exit proc_basic_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    pr_info("proc_basic: unloaded\n");
}

module_init(proc_basic_init);
module_exit(proc_basic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Basic procfs driver example");
