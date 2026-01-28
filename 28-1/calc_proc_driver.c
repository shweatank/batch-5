#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "proc_basic"
#define BUF_LEN   128

struct calc
{
int a;
int b;
int res;
char s[20];
};


static char proc_buffer[BUF_LEN];
static size_t proc_buffer_len;

static ssize_t proc_read(struct file *file,
                         char __user *user_buf,
                         size_t count,
                         loff_t *ppos)
{


if(count>sizeof(struct calc))
return -EFAULT;

if(copy_to_user(user_buf,proc_buffer,sizeof(struct calc)))
return -EFAULT;

return sizeof(struct calc);
}


static ssize_t proc_write(struct file *file,
                          const char __user *user_buf,
                          size_t count,
                          loff_t *ppos)
{
struct calc *k;
    if (count != sizeof(struct calc))
        return -EFAULT;

    if (copy_from_user(proc_buffer, user_buf, count))
        return -EFAULT;

    k=(struct calc*)proc_buffer;
      if(strcmp(k->s,"add")==0)
     k->res=k->a+k->b;  
      else if(strcmp(k->s,"sub")==0)
	      k->res=k->a-k->b;  
      else  if(strcmp(k->s,"mul")==0)
	      k->res=k->a*k->b;  
      else  if(strcmp(k->s,"div")==0)
	      k->res=k->a/k->b;  
      else  if(strcmp(k->s,"mod")==0)
	      k->res=k->a%k->b;  
  
    pr_info("proc_basic:add=%d\n", k->res);

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
