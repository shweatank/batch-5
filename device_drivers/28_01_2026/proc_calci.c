#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "proc_calci"
#define BUF_LEN   128

//static char proc_buffer[BUF_LEN];
static size_t proc_buffer_len;
struct calci{
        char ch;
        int a;
        int b;
        int res;
}c;


void reverse(char *p,int i,int j);


static ssize_t proc_read(struct file *file,
                         char __user *user_buf,
                         size_t count,
                         loff_t *ppos)
{
    return simple_read_from_buffer(user_buf,
                                   count,
                                   ppos,
                                   &c,
                                   proc_buffer_len);
}

static ssize_t proc_write(struct file *file,
                          const char __user *user_buf,
                          size_t count,
                          loff_t *ppos)
{
    if (count > BUF_LEN - 1)
        count = BUF_LEN - 1;

    if (copy_from_user(&c, user_buf, count))
        return -EFAULT;

   // proc_buffer[count] = '\0';
    //proc_buffer_len = count;
    proc_buffer_len = sizeof(c);

    pr_info("proc_calci:received:%d %d\n",c.a,c.b);
//	reverse(proc_buffer,0,count);
	//cal();
	switch(c.ch){
		case 'a':
			c.res=c.a+c.b;
			break;
		case 's':c.res=c.a-c.b;break;
	}
    pr_info("Result:%d\n", c.res);
    return count;
}

static const struct proc_ops proc_fops = {
    .proc_read  = proc_read,
    .proc_write = proc_write,
};

static int __init proc_basic_init(void)
{
    proc_create(PROC_NAME, 0666, NULL, &proc_fops);
    pr_info("proc_calci: loaded\n");
    return 0;
}

static void __exit proc_basic_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    pr_info("proc_calci: unloaded\n");
}

module_init(proc_basic_init);
module_exit(proc_basic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Basic procfs driver example");

