#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/io.h>
#include<linux/delay.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/platform_device.h>
#include<linux/of.h>
#include<linux/of_address.h>

#define spi_base 0xFE204000
#define CS 0x00
#define FIFO 0x04
#define CLK 0x08
#define DLEN 0x0c

#define DEVICE "spi_data"
static int major;
static void __iomem *base;
void spi_init(void){
	
	writel(0x0,base+CS);

	writel((1<<3)|(1<<2)|(3<<4),CS+base);//transferring 8bits at a time,CPOL=1,CPHA=1,clear TX,RX FIFO

	writel(100,base+CLK);//setting clock

	writel(readl(base+CS)|(1<<7),base+CS);//enabling transmision
}

static ssize_t spi_write(struct file*file,const char __user *usr,size_t len,loff_t *off){

char ch;
size_t i;
pr_info("Write is called\n");
	for(i=0;i<len;i++){
		if(copy_from_user(&ch,&usr[i],1))
			return -EFAULT;
		pr_info("write data =%c\n",ch);
		while(!(readl(base+CS)&(1<<18)));
		writel(ch,base+FIFO);
	}
	pr_info("Write ..........bye\n");
	return len;
}


static ssize_t spi_read(struct file*file,char __user *usr,size_t len,loff_t *off){
	char ch;
	size_t i;
	pr_info("read---hai.......\n");
	for(i=0;i<len;i++){

		while(!(readl(base+CS)&(1<<17))){
			pr_info("................\n");
		}

		ch=(readl(base+FIFO))&0xFF;
		pr_info("read data=%c\n",ch);

		if(copy_to_user(&usr[i],&ch,1))
			return -EFAULT;
	}
	pr_info("read---bye..........\n");
	return len;
}

static struct file_operations fops={
	.owner=THIS_MODULE,
	.read=spi_read,
	.write=spi_write,
};


static int spi_probe (struct platform_device *pdev){
	struct resource *res;
	res=platform_get_resource(pdev,IORESOURCE_MEM,0);

	base=devm_ioremap_resource(&pdev->dev,res);
	major=register_chrdev(0,DEVICE,&fops);
spi_init();
	pr_info("major number=%d\n",major);
	pr_info("Driver loaded\n");
	return 0;
}

static int spi_remove (struct platform_device *pdev){

	unregister_chrdev(major,DEVICE);
	writel(readl(base+CS)& ~(1<<7), base+CS);
	pr_info("Driver unloaded\n");
	return 0;
}

static const struct of_device_id spi_of_match[]= {
	{ .compatible = "my,spi_driver"},
	{},
};

MODULE_DEVICE_TABLE(of,spi_of_match);

static struct platform_driver spi_driver= {
	.probe=spi_probe,
	.remove=spi_remove,
	.driver={
		.name	="spi_driver",
		.of_match_table=spi_of_match,
	},
};

module_platform_driver(spi_driver);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("KISHORE");
MODULE_DESCRIPTION("Basic SPI Driver");

