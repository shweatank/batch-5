#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define I2C_PHY 0xFe205000
#define I2C_SIZE 0x20

#define CONTROL_REG 0x00
#define STAT_REG 0x04
#define SL_REG  0x0c
#define D_REG   0x08
#define FIFO_REG 0x10
#define CLK_REG 0x14
#define DONE (1<<1)
#define START (1<<7)
#define READ (1<<0)
#define ACK (1<<8)
#define DRIVER_NAME "i2c_file"

static int major;
static void __iomem* i2c_base;
static char k_buf[128]; 
struct info
{
char sa;
char data;
char mr;
};
static struct info *ptr;
static void i2c_write(unsigned char sa,unsigned char mr,unsigned char data)
{
	u32 status;
	writel(START,i2c_base+CONTROL_REG);

	writel(sa,i2c_base+SL_REG);
	while(!(readl(i2c_base+STAT_REG)&DONE));

	writel(mr,i2c_base+FIFO_REG);
	while(!(readl(i2c_base+STAT_REG)&DONE));

	writel(data,i2c_base+FIFO_REG);
	while(!(readl(i2c_base+STAT_REG)&DONE));
}
static unsigned char i2c_read(unsigned char sa,unsigned char mr)
{
            
	char data;
		writel(START,i2c_base+CONTROL_REG);

	writel(sa^1,i2c_base+SL_REG);
	while(!(readl(i2c_base+STAT_REG)&DONE));

	writel(mr,i2c_base+FIFO_REG);
	while(!(readl(i2c_base+STAT_REG)&DONE));
	
	writel(START,i2c_base+CONTROL_REG);

	data=readl(i2c_base+D_REG);
	while(!(readl(i2c_base+STAT_REG)&DONE));
return data;
}
static ssize_t write(struct file*file,const char __user*buf,size_t count,loff_t *offset )
{
	if(count!=sizeof(struct info))
		return 0;
	if(copy_from_user(k_buf,buf,sizeof(struct info)))
		return -EFAULT;
	ptr=(struct info*)k_buf;
	i2c_write(ptr->sa,ptr->mr,ptr->data);
	pr_info("i2c_write done\n");
return 0;
}
static ssize_t read(struct file*file,char __user*buf,size_t count,loff_t *offset )
{
char read_data;
read_data=i2c_read(ptr->sa,ptr->mr);
ptr->data=read_data;
if(copy_to_user(buf,ptr,sizeof(struct info)))
	return -EFAULT;
pr_info("i2c_read %c completed\n",read_data);
return sizeof(char);
}
static struct file_operations fops={
        .owner=THIS_MODULE,
	.write=write,
	.read=read
};
static int __init i2c_init(void)
{
major=register_chrdev(0,DRIVER_NAME,&fops);
if(major<0)
{
pr_info("major number not allocated\n");
return -EFAULT;
}
	
	i2c_base=ioremap(I2C_PHY,I2C_SIZE);
if(!i2c_base)
{
pr_info("ioremap failed\n");
return -EFAULT;
}
writel((1<<5),i2c_base+CONTROL_REG);
writel(1500,i2c_base+CLK_REG);
pr_info("sudo mknod /dev/%s c %d 0",DRIVER_NAME,major);
return 0;
}
static void __exit i2c_exit(void)
{
	unregister_chrdev(major,DRIVER_NAME);
	iounmap(i2c_base);
	pr_info("exited normally\n");
}

module_init(i2c_init);
module_exit(i2c_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Saikumar");
MODULE_DESCRIPTION("Simple I2C Register Driver");




