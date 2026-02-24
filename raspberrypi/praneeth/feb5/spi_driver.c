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

static unsigned char spi_transfer(unsigned char data)
{
	while(!(readl(spi_base+CS)&(1<<18)));
	writel(data,spi_base+FIFO);
	while(!(readl(spi_base+CS)&(1<<17)));
	return (readl(spi_base+FIFO)&0XFF);
}

static int __init spi_init(void)
{
	spi_base=ioremap(SPI_BASE,SPI_SIZE);
	if(!spi_base)
	{
		pr_err("ioremap failure\n");
		return -ENOMEM;
	}
	//disable spi
	writel(0x00,spi_base+CS);
	//clearing fifos
	writel(3<<4,spi_base+CS);
	//spi clock
	writel(65536,spi_base+CLK);

	//chip select clk pol clk phase config in cs 
	int cs=readl(spi_base+CS);
	cs &=~(0X0F<<0);
	writel(cs,spi_base+CS);
	//transfer active
	writel((1<<7),spi_base+CS);

	char ch=spi_transfer('A');
	pr_info("data from slave : %x\n",ch);
}
static void __exit spi_exit(void)
{
	
}

module_init(spi_init);
module_exit(spi_exit);

MODULE_LISENCE("GPL");
