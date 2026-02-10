#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/errno.h>

#define PROC_NAME "proc_basic"
#define BUF_LEN   128

typedef struct result{
	int a;
	int b;
	char ch;
	int res;
}re;


static char proc_buffer[BUF_LEN];
static size_t proc_buffer_len;
static re r,w;

static ssize_t proc_read(struct file *file,
                         char __user *user_buf,
                         size_t count,
                         loff_t *ppos)
{
    return simple_read_from_buffer(user_buf,
                                   count,
                                   ppos,
				   &w,
				   sizeof(re));
}

static ssize_t proc_write(struct file *file,
                          const char __user *user_buf,
                          size_t count,
                          loff_t *ppos)
{
    if (count > sizeof(re))
	    count = sizeof(re);
	if(copy_from_user(&r,user_buf,sizeof(re))){	
		return -EFAULT;
	}
	pr_info("received %d, %d\n",r.a,r.b);
	if(r.ch=='+')
		w.res=r.a+r.b;
	else if(r.ch=='-')
                w.res=r.a-r.b;
	else if(r.ch=='*')
                w.res=r.a*r.b;
	else if(r.ch=='/'){
		if(r.b==0)
			w.res=0;
		else
                	w.res=r.a/r.b;
	}
/*	memset(proc_buffer,0,BUF_LEN);
	int i=0,te=r.res;

	while(te){
		proc_buffer[i++]=te%10+'0';
		te/=10;
	}
	proc_buffer[i]='\0';
	//pr_info("proc_basic: received \%s\"\n",proc_buffer);
    char *p,*q,t;
   // int len=count;
   // if(len>0 && proc_buffer[len-1]=='\n')
//	    len--;
  //  proc_buffer[len]='\0';
    p=proc_buffer;
    q=proc_buffer+i-1;
    while(p<q){
            t=*p;*p=*q;*q=t;
            p++,q--;
    }
    //proc_buffer[count] = '\0';
    pr_info("computed result is %s\n",proc_buffer);
  */
      	proc_buffer_len = count;

    //pr_info("proc_basic: received \"%s\"\n", proc_buffer);

    return count;
}

static const struct proc_ops proc_fops = {
    .proc_read  = proc_read,
    .proc_write = proc_write,
};

static int __init proc_basic_init(void)
{
//	r=kmalloc(sizeof(re),GFP_KERNEL);
//	i(!r)
//		return -ENOMEM;
    proc_create(PROC_NAME, 0666, NULL, &proc_fops);
    pr_info("proc_basic: loaded\n");
    return 0;
}

static void __exit proc_basic_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    pr_info("proc_basic: unloaded\n");
  //  kfree(r);
}

module_init(proc_basic_init);
module_exit(proc_basic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Basic procfs driver example");
