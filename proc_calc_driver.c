#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "proc_calc"
#define BUF_LEN   128
struct st{int a;int b;int rout;char op;};
static struct st proc_buff,res;
static char proc_buffer[BUF_LEN];
static size_t proc_buffer_len;
int result=0;

static int proc_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "proc_calc: device opened\n");
    return 0;
}

/*
 * Called when user closes /dev/basic_char
 */
static int proc_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "proc_calc: device closed\n");
    return 0;
}

static ssize_t proc_read(struct file *file,
                         char __user *user_buf,
                         size_t count,
                         loff_t *ppos)
{
    return simple_read_from_buffer(user_buf,
                                   count,
                                   ppos,
                                   &res,
                                   sizeof(struct st));
}

static ssize_t proc_write(struct file *file,
                          const char __user *user_buf,
                          size_t count,
                          loff_t *ppos)
{
    if (count > BUF_LEN - 1)
        count = BUF_LEN - 1;
	struct st *user_val=(struct st *)user_buf;

    if (copy_from_user(&proc_buff, user_val, count))
        return -EFAULT;
	
    pr_info("proc_calc: received a:%d b:%d op:%c\n", proc_buff.a,proc_buff.b,proc_buff.op);
    switch(proc_buff.op)
    {
	    case 'a':res.rout=proc_buff.a+proc_buff.b;break;
	    case 's':res.rout=proc_buff.a-proc_buff.b;break;
	    case 'm':res.rout=proc_buff.a*proc_buff.b;break;
	    case 'd':res.rout=proc_buff.b?proc_buff.a/proc_buff.b:0;break;
	    default:pr_info("invalid option");
    }
    pr_info("result:%d\n",res.rout);
	
    return count;
}

static struct proc_ops proc_fops={
    .proc_open=proc_open,
    .proc_release=proc_release,
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
/*
echo "hellooo" > /proc/proc_basic
cat /proc/proc_basic
hellooo
*/

