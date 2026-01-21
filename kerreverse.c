/*
 * basic_char_driver.c
 *
 * Educational Linux character device driver.
 * Demonstrates:
 *  - module init / exit
 *  - character device registration
 *  - basic file operations (open, read, write, release)
 *
 * No real hardware is accessed.
 */

#include <linux/module.h>   // Core module macros
#include <linux/kernel.h>   // printk()
#include <linux/init.h>     // __init, __exit
#include <linux/fs.h>       // register_chrdev, file_operations
#include <linux/uaccess.h>  // copy_to_user, copy_from_user
//#include<string.h>

#define DEVICE_NAME "cal"
#define BUF_SIZE    128

static int major_number;
static char kernel_buffer[BUF_SIZE];
static int buffer_size;
static int num1,num2,result;

//static char user[100]={0};

//static void rev(void);
static void operation(void);
static void rev(void)
{
	 int i,j;
        int n=0;

       for(n=0;kernel_buffer[n]!='\0';n++);

//	kernel_buffer[n]='\0';

        for(i=0,j=n-1;i<j;i++,j--)
        {
                  char temp=kernel_buffer[i];
                  kernel_buffer[i]=kernel_buffer[j];
                  kernel_buffer[j]=temp;
        }

}


static void operation(void)
{
	  int i=0;
	  int flag=1;
	  char opr;


	  while(kernel_buffer[i]!='\0')
	  {
		  if(kernel_buffer[i]>='0'&&kernel_buffer[i]<='9'&& flag==1)
		  {
			  num1=num1*10+kernel_buffer[i]-'0';
		  }
		  else if(kernel_buffer[i]=='+'||kernel_buffer[i]=='-'||kernel_buffer[i]=='*'||kernel_buffer[i]=='/')
		  {
			    flag=0;
			    opr=kernel_buffer[i];
		  }
		  else if(kernel_buffer[i]>='0'&&kernel_buffer[i]<='9')
		  {
			  num2=num2*10+kernel_buffer[i]-'0';
		  }


		  i++;

	  }

	   switch(opr)
	   {
		   case '+':result=num1+num2;
			   break;
		   case '-': result=num1-num2;
			     break;
		   case '*':result=num1*num2;
			    break;
		   case '/': result=num1/num2;
	    }



	 // printk(KERN_INFO "num2=%d\n",num2);

	  printk(KERN_INFO "result=%d\n",result);

	  int l=0;


	 /*  while(temp>0)
	   {
		    var=var*10+(temp%10);
		    temp=temp/10;
	   }

          result=var;
	  */
   
	  while(result>0)
	  {

		  kernel_buffer[l++]=(result%10)+'0';
		  result=result/10;
	  }

	  num1=0;
	  num2=0;
	  result=0;

	  kernel_buffer[l]='\0';

	  rev();


}		 


/*
 * Called when user opens /dev/basic_char
 */
static int basic_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "basic_char: device opened\n");
    return 0;
}

/*
 * Called when user closes /dev/basic_char
 */
static int basic_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "basic_char: device closed\n");
    return 0;
}

/*
 * Called when user reads from /dev/basic_char
 */
static ssize_t basic_read(struct file *file,
                          char __user *user_buffer,
                          size_t count,
                          loff_t *offset)
{
    int bytes_to_copy;

    /*
     * If offset is beyond data, return 0 (EOF)
     */
    if (*offset >= buffer_size)
        return 0;

    bytes_to_copy = min(count, (size_t)(buffer_size - *offset));

    /*
     * Copy data from kernel space to user space
     */
    if (copy_to_user(user_buffer,
                     kernel_buffer + *offset,
                     bytes_to_copy))
        return -EFAULT;


     //   kernel_buffer[bytes_to_copy+1]='\0';


    *offset += bytes_to_copy;

    printk(KERN_INFO "basic_char: read %d bytes\n", bytes_to_copy);
    return bytes_to_copy;
}

/*
 * Called when user writes to /dev/basic_char
 */
static ssize_t basic_write(struct file *file,
                           const char __user *user_buffer,
                           size_t count,
                           loff_t *offset)
{
    int bytes_to_copy;

    bytes_to_copy = min(count, (size_t)BUF_SIZE);

    /*
     * Copy data from user space to kernel space
     */
    if (copy_from_user(kernel_buffer,
                       user_buffer,
                       bytes_to_copy))
        return -EFAULT;

    //  printk(KERN_INFO "in write:\n");
    

      kernel_buffer[bytes_to_copy]='\0';


        operation();


    //   rev();
     
      

     buffer_size = bytes_to_copy;

    printk(KERN_INFO "basic_char: wrote %d bytes\n", bytes_to_copy);
    return bytes_to_copy;
}

/*
 * File operations structure
 * This connects system calls to driver functions
 */
static struct file_operations basic_fops = {
    .owner   = THIS_MODULE,
    .open    = basic_open,
    .read    = basic_read,
    .write   = basic_write,
    .release = basic_release,
};

/*
 * Module initialization
 */
static int __init basic_char_init(void)
{
    /*
     * Register character device
     * 0 → dynamic major number
     */
    major_number = register_chrdev(0, DEVICE_NAME, &basic_fops);
    if (major_number < 0) {
        printk(KERN_ERR "basic_char: failed to register device\n");
        return major_number;
    }

    printk(KERN_INFO "basic_char: loaded\n");
    printk(KERN_INFO "basic_char: major number = %d\n", major_number);
    printk(KERN_INFO "Create device node with:\n");
    printk(KERN_INFO "mknod /dev/%s c %d 0\n", DEVICE_NAME, major_number);

    return 0;
}

/*
 * Module cleanup
 */
static void __exit basic_char_exit(void)
{
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "basic_char: unloaded\n");
}

/* Kernel module macros */
module_init(basic_char_init);
module_exit(basic_char_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prashant");
MODULE_DESCRIPTION("Educational basic character driver with file operations");
