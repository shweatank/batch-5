#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/device.h>
#include <linux/delay.h>

#define SPI_BASE 0XFE204000
#define SPI_SIZE 0X18

#define CS   0X00
#define FIFO 0X04
#define CLK  0x08
#define DLEN 0X0C
#define LTOH 0X10
#define DC   0X14

static void __iomem *spi_base;


static unsigned char spi_data_transfer(unsigned char data)
{
	while((readl(spi_base+CS)&(1<<18))==0);
	writel(data,spi_base+FIFO);
	while((readl(spi_base+CS)&(1<<17))==0);
	char ch=readl(spi_base+FIFO);
	return ch;
}

static int __init spi_init(void)
{
	spi_base=ioremap(SPI_BASE,SPI_SIZE);
	if(!spi_base)
	{
		pr_err("ioremap_failure\n");
		return 0;
	}
	pr_info("ioremap sucuss\n");
	//initialisation of spi
	int cs =readl(spi_base+CS);
	cs |=((3<<4) | (1<<2) | (1<<3));
	writel(cs,spi_base+CS);
	//configuring spi clk
	writel(readl(spi_base+CS)|(1<<5),spi_base+CS);
	char ch=spi_data_transfer('A');
	pr_info("data from slave : %c\n",ch);
	return 0;
}

static void __exit spi_exit(void)
{
	writel(0x00,spi_base+CS);
	iounmap(spi_base);
}

module_init(spi_init);
module_exit(spi_exit);

MODULE_LICENSE("GPL");

