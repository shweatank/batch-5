#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/io.h>
#include<linux/delay.h>

#define uart_basic 0xFE201000
#define data 0x00
#define flag 0x18
#define IBRD 0x24
#define FBRD 0x28
#define LCR 0x2c
#define CR 0x30


static void __iomem *base;
static int __init uart_init(void){

	base=ioremap(uart_basic,0x100);

	writel(0x0,base+CR);	
	writel(26,base+IBRD);
	writel(3,base+FBRD);
	writel(7<<4,base+LCR);
	writel((1<<0)|(1<<8),base+CR);
	//	writel((1<<8),base+CR);
	while(readl(base+flag)&(1<<5));
	writel('A',base+data);
	return 0;

}

static void __exit uart_exit(void){

	writel(0x0,base+CR);
	if(base){
		iounmap(base);
	}

}

module_init(uart_init);
module_exit(uart_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("KISHORE");
MODULE_DESCRIPTION("Basic UART driver");
