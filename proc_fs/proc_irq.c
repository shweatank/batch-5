#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/errno.h>

#define PROC_NAME "proc_irq"
#define BUF_LEN   128
#define DEVICE_NAME "irq_with_proc"
#define KBD_IRQ 1
#define KBD_DATA_PORT 0x60

static const char *keymap[]={
	[0x18]="O",[0x1E]="A",[0x1F]="S",[0x20]="D",[0x32]="M"
};

typedef struct result{
	int a;
	int b;
	int res;
}re;


static char proc_buffer[BUF_LEN];
static size_t proc_buffer_len;
static struct work_struct irq_work;
static struct workqueue_struct *my_wq;
re r,*w;
char *ch;

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
                                  // proc_buffer,
                                  // proc_buffer_len);
}

static ssize_t proc_write(struct file *file,
                          const char __user *user_buf,
                          size_t count,
                          loff_t *ppos)
{
    if (count > sizeof(re))
	    count = sizeof(re);
	if(copy_from_user(&r,(re *)user_buf,sizeof(re)))
			return -EFAULT;
	pr_info("received %d, %d\n",r.a,r.b);
	/*if(r.ch=='+')
		r.res=r.a+r.b;
	else if(r.ch=='-')
                r.res=r.a-r.b;
	else if(r.ch=='*')
                r.res=r.a*r.b;
	else if(r.ch=='/'){
		if(r.b==0)
			r.res=0;
		else
                	r.res=r.a/r.b;
	}
//	*ppos+=count;
	memset(proc_buffer,0,BUF_LEN);
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
    pr_info("computed result is %s\n",proc_buffer);*/
    proc_buffer_len = count;

    //pr_info("proc_basic: received \"%s\"\n", proc_buffer);

    return count;
}
static void work_handler(struct work_struct *work)
{
    pr_info("work_handler: running in process context\n");
        switch(*ch){
            case 'A':r.res=r.a+r.b;
                     pr_info("Sum res is %d\n",r.res);
                     break;
        case 'S':r.res=r.a-r.b;
                 if(r.res<0)
                         r.res=-(r.res);
                     pr_info("Diff res is %d\n",r.res);
                     break;
        case 'M':r.res=r.a*r.b;
                     pr_info("Mul res is %d\n",r.res);
                     break;
        case 'D':if(r.b==0)
                         r.res=0;
                 else
                         r.res=r.a/r.b;
                     pr_info("Div res is %d\n",r.res);
                     break;
        case 'O':if(r.b==0)
                         r.res=0;
                 else
                         r.res=r.a%r.b;
                     pr_info("Mod res is %d\n",r.res);
                     break;
    }
	 memset(proc_buffer,0,BUF_LEN);
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
	//          len--;
  	//  proc_buffer[len]='\0';
    	p=proc_buffer;
    	q=proc_buffer+i-1;
    	while(p<q){
            t=*p;*p=*q;*q=t;
            p++,q--;
    	}
    	//proc_buffer[count] = '\0';
    	pr_info("computed result is %s\n",proc_buffer);
    	//proc_buffer_len = count;


    /* Heavy work is allowed */
   // msleep(1000);

    pr_info("work_handler: completed\n");
}


static irqreturn_t keyboard_isr(int irq, void *dev_id)
{
    unsigned char scancode;
    bool released;
    const char *key;

    scancode = inb(KBD_DATA_PORT);

    released = scancode & 0x80;
    scancode &= 0x7F;

    key = keymap[scancode];

    if (!key)
        key = "UNKNOWN";
	ch=key;
	queue_work(my_wq,&irq_work);

}


static const struct proc_ops proc_fops = {
    .proc_read  = proc_read,
    .proc_write = proc_write,
};

static int __init proc_basic_init(void)
{
    proc_create(PROC_NAME, 0666, NULL, &proc_fops);
    pr_info("proc_basic: loaded\n");
    int ret;
    printk(KERN_INFO "kbd_irq: Initializing keyboard IRQ driver\n");

    ret = request_irq(KBD_IRQ,
                      keyboard_isr,
                      IRQF_SHARED,
                      "kbd_irq_key_driver",
                      (void *)keyboard_isr);

    if (ret) {
        printk(KERN_ERR "kbd_irq: Failed to register IRQ %d\n", KBD_IRQ);
        return ret;
    }

    printk(KERN_INFO "kbd_irq: Keyboard IRQ registered successfully\n");

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
