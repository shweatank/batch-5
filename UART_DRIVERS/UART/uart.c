#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/io.h>
#include<linux/delay.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#define uart_basic 0xFE201000
#define data 0x00
#define flag 0x18
#define IBRD 0x24
#define FBRD 0x28
#define LCR 0x2c
#define CR 0x30

static dev_t devno;
static struct cdev uart_cdev;
static struct class *uart_class;

static void __iomem *base;
static int __init uart_hw_init(void){

	writel(0x0,base+CR);	
	writel(26,base+IBRD);
	writel(3,base+FBRD);
	writel((3<<5)|(1<<4),base+LCR);
	writel((1<<0)|(1<<8)|(1<<9),base+CR);
	while(readl(base+flag)&(1<<5));
	writel('A',base+data);
	char ch;
	while(readl(base+flag)&(1<<4));
	ch=readl(base+data)&0xFF;

	return 0;

}
static ssize_t uart_write(struct file*file,const char __user *buf,size_t len,loff_t*off){
	char ch;
	size_t i;
	for(i=0;i<len;i++){
		copy_from_user(&ch,&buf[i],1);
		while(readl(base+flag)&(1<<5))
			cpu_relax();
		writel(ch,base+data);
	}
	return len;
}

static ssize_t uart_read(struct file*file,char __user *buf,size_t len,loff_t*off){
	char ch;
	size_t i;
	for(i=0;i<len;i++){
		while(readl(base+flag)&(1<<4))
			cpu_relax();
		ch=readl(base+data)&0xFF;
		copy_to_user(&buf[i],&ch,1);
	}
	return i;
}

static struct file_operations uart_fops={
	.owner=THIS_MODULE,
	.read=uart_read,
	.write=uart_write,
};

static int __init uart_init(void){
	base=ioremap(uart_basic,0x100);
	alloc_chrdev_region(&devno,0,1,"/dev/rpi_uart");
	cdev_init(&uart_cdev,&uart_fops);
	cdev_add(&uart_cdev,devno,1);
	uart_class=class_create(THIS_MODULE,"rpi_uart");
	device_create(uart_class,NULL,devno,NULL,"rpi_uart");
	uart_hw_init();
	return 0;
}
static void __exit uart_exit(void){

	writel(0x0,base+CR);
	device_destroy(uart_class,devno);
	class_destroy(uart_class);
	cdev_del(&uart_cdev);
	unregister_chrdev_region(devno,1);
	iounmap(base);
}

module_init(uart_init);
module_exit(uart_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("KISHORE");
MODULE_DESCRIPTION("Basic UART driver");
