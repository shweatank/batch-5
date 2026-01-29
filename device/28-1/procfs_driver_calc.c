#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "proc_basic"
#define BUF_LEN   128

static char proc_buffer[BUF_LEN];
static size_t proc_buffer_len;
struct cal
{
	int a;
	int b;
	long res;
	char ch;
};

struct cal c;


int Separate(char *,size_t );
//void String(char *,size_t ,int );

int Separate(char *str,size_t count)
{
	int i=0,j=count-2;
	char ch=str[j];
	int f=0;
	int a=0,b=0,res=0;
	for(i=0;i<j-1;i++)
	{
		while(str[i]!=',')
		{
			if(f==0)
			a=(a*10)+str[i]-48;
			if(f==1)
			b=(b*10)+str[i]-48;
			i++;
		}
		f=1;
	}
	printk("%c\n",ch);
	switch(ch)
	{
		case'+':res=a+b;
			break;
		case'-':res=a-b;
			break;
		case'*':res=a*b;
			break;
	}
	return res;
}
/*void String(char *str,size_t count,int res)
{
	int i=0,nflag=0;
	while(str[i])
	{
		str[i]='\0';
		i++;
	}
	i=0;
	if(res<0)
	{
		res=-res;
		nflag=1;
	}
	while(res)
	{
		str[i]=(res%10)+48;
		res/=10;
		i++;
	}
	if(nflag==1)
	{
		str[i]='-';
		i++;
	}
	str[i]='\0';

}*/

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
}

static ssize_t proc_write(struct file *file,
                          const char __user *user_buf,
                          size_t count,
                          loff_t *ppos)
{
    if (count > BUF_LEN - 1)
        count = BUF_LEN - 1;

    if (copy_from_user(proc_buffer, user_buf, count))
        return -EFAULT;

    proc_buffer[count] = '\0';
    proc_buffer_len = count;

    pr_info("proc_basic: received \"%s\"\n", proc_buffer);
    int res=Separate(proc_buffer,count);
    printk("res is %d \n",res);
  // String(proc_buffer,count,res);
   // pr_info("proc_basic: received %s\n",proc_buffer);

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
