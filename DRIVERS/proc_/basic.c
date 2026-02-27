#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "proc_basic"
#define BUF_LEN   128
struct st{
	int a;
	int b;
	char ch;
	int res;
};

//static struct st *k_buff;

static char *proc_buffer;
static size_t proc_buffer_len;

static ssize_t proc_read(struct file *file,
		char __user *user_buf,
		size_t count,
		loff_t *ppos)
{
//char buf[128];
//int len=snprintf(buf,sizeof(buf),"%d\n",proc_buffer->res);

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
struct st v;
	if (count > BUF_LEN)
		count = BUF_LEN;

	if (copy_from_user(proc_buffer,user_buf, count))
		return -EFAULT;

	switch(proc_buffer->ch){
		case '+':proc_buffer->res=proc_buffer->a+proc_buffer->b;break;
		case '-':proc_buffer->res=proc_buffer->a-proc_buffer->b;break;
		case '*':proc_buffer->res=proc_buffer->a*proc_buffer->b;break;
		case '/':proc_buffer->res=proc_buffer->a/proc_buffer->b;break;
	}

	//proc_buffer[count] = '\0';
	proc_buffer_len = sizeof(struct st);

	//	pr_info("proc_basic: received \"%s\"\n", proc_buffer);
	pr_info("Result is %d\n",proc_buffer->res);

	return count;
}

static const struct proc_ops proc_fops = {
	.proc_read  = proc_read,
	.proc_write = proc_write,
};

static int __init proc_basic_init(void)
{
	proc_buffer=kmalloc(sizeof(struct st),GFP_KERNEL);
	proc_create(PROC_NAME, 0666, NULL, &proc_fops);
	pr_info("proc_basic: loaded\n");
	return 0;
}

static void __exit proc_basic_exit(void)
{
	kfree(proc_buffer);
	remove_proc_entry(PROC_NAME, NULL);
	pr_info("proc_basic: unloaded\n");
}

module_init(proc_basic_init);
module_exit(proc_basic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Basic procfs driver example");
