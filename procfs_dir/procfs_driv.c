#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/module.h>   // Core module macros
#include <linux/kernel.h>   // printk()
#include <linux/init.h>     // __init, __exit
#include <linux/fs.h>       // register_chrdev, file_operations
#include <linux/uaccess.h>  // copy_to_user, copy_from_u
#include <linux/interrupt.h>
#include <linux/io.h>


#define PROC_NAME "proc_basic"
#define BUF_LEN   128

#define KBD_IRQ        1
#define KBD_DATA_PORT  0x60

static char proc_buffer[BUF_LEN];
static size_t proc_buffer_len;

DECLARE_WAIT_QUEUE_HEAD(my_wq);    // reating wait que;
				  
  static int data_ready=0;



static char *key;

static struct  var
{
	char op;
	int a;
	int b;
	int res;
}var;

static int flag=0;


static void operations(void);
static const char *keymap[128] = {
    [0x01] = "ESC",
    [0x02] = "1",   [0x03] = "2",   [0x04] = "3",
    [0x05] = "4",   [0x06] = "5",   [0x07] = "6",
    [0x08] = "7",   [0x09] = "8",   [0x0A] = "9",
    [0x0B] = "0",
    [0x0C] = "-",   [0x0D] = "=",
    [0x0E] = "BACKSPACE",
    [0x0F] = "TAB",
    [0x10] = "Q",   [0x11] = "W",   [0x12] = "E",
    [0x13] = "R",   [0x14] = "T",   [0x15] = "Y",
    [0x16] = "U",   [0x17] = "I",   [0x18] = "O",
    [0x19] = "P",
    [0x1C] = "ENTER",
    [0x1D] = "CTRL",
    [0x1E] = "A",   [0x1F] = "S",   [0x20] = "D",
    [0x21] = "F",   [0x22] = "G",   [0x23] = "H",
    [0x24] = "J",   [0x25] = "K",   [0x26] = "L",
    [0x2C] = "Z",   [0x2D] = "X",   [0x2E] = "C",
    [0x2F] = "V",   [0x30] = "B",   [0x31] = "N",
    [0x32] = "M",
    [0x39] = "SPACE"
};
static irqreturn_t keyboard_isr(int irq, void *dev_id)
{
    unsigned char scancode;
    bool released;

    scancode = inb(KBD_DATA_PORT);

   // released = scancode & 0x80;
   // scancode &= 0x7F;

    if(scancode & 0x80)
            return IRQ_HANDLED;


    key = keymap[scancode];

    if (!key)
        key = "UNKNOWN";

   // printk("key=%s\n",key);

          if(!(strcmp(key,"9")))
	  {
		  data_ready=1;
		  wake_up_interruptible(&my_wq);
	  }

		  return IRQ_HANDLED;
}
//static void operations(void);

static void operations(void)
{
	   switch(var.op)
	   {
		   case '+': var.res=var.a+var.b;
			     break;
		   case '-': var.res=var.a-var.b;
			     break;
		   case '*': var.res=var.a*var.b;
			     break;
		   case '/': var.res=var.a/var.b;
			     break;
	}

	   pr_info("result=%d\n",var.res);

}

static ssize_t proc_read(struct file *file,
                         char __user *user_buf,
                         size_t count,
                         loff_t *ppos)
{
  /*  return simple_read_from_buffer(user_buf,
                                   sizeof(var),
                                   ppos,
                                   &var,
     				   proc_buffer_len);*/

	wait_event_interruptible(my_wq,data_ready==1);

	data_ready=0;

       operations();

	 if (copy_to_user(__user user_buf,&var,sizeof(var)))
        return -EFAULT;

	 return sizeof(var);

}

static ssize_t proc_write(struct file *file,
                          const char __user *user_buf,
                          size_t count,
                          loff_t *ppos)
{
    if (count > BUF_LEN - 1)
        count = BUF_LEN - 1;

    if (copy_from_user(&var, user_buf, sizeof(var)))
        return -EFAULT;

  //  proc_buffer[count] = '\0';
    //proc_buffer_len = count;

 //   pr_info("proc_basic: received \"%s\"\n", proc_buffer);
 
    pr_info("revevied=%d %d \n",var.a,var.b);

   // operations();

    return count;
}

static const struct proc_ops proc_fops = {
    .proc_read  = proc_read,
    .proc_write = proc_write,
};

static int __init proc_basic_init(void)
{

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


    proc_create(PROC_NAME, 0666, NULL, &proc_fops);
    pr_info("proc_basic: loaded\n");
    return 0;
}

static void __exit proc_basic_exit(void)
{

  free_irq(KBD_IRQ, (void *)keyboard_isr);
   printk(KERN_INFO "kbd_irq: Keyboard IRQ driver unloaded\n");

    remove_proc_entry(PROC_NAME, NULL);
    pr_info("proc_basic: unloaded\n");
}

module_init(proc_basic_init);
module_exit(proc_basic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Basic procfs driver example");
