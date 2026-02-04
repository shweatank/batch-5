#include<stdio.h>

#define UART0_BASE_PHYS 0x7e201000
#define UART0_SIZE	0X90

#define DR   0X00
#define IBRD 0X24
#define FBRD 0X28
#define LCRH 0X2C
#define CR   0X30
#define ICR  0X44
#define FR   0X18

#define BAUD 115200
#define PCLK 48000
#define DIVISOR (PCLK/(16*BAUD))

static void uart_putc(char c)
{
	while(readl(uart_base+FR)&(1<<5));	//checking whether tx buff full
	writel(c,uart_base+DR);
}

static char uart_getc(void)
{
	while(readl(uart_base+FR)&(1<<4));
	return readl(uart_base+DR)&0XFF;
}

static void uart_puts(char *str)
{
	while(*str)
	{
		uart_putc(*str++);
	}
}


static void __init uart_init(void)
{
	pr_info("uart0 driver initialisation\n");
	uart_base=ioremap(UART_BASE_PHYS,UART0_SIZE);
	if(!uart_base)
	{
		pr_info("ioremap failed\n");
		return -ENOMEM;
	}
	pr_info("ioremap success\n");
	writel(0x0,uart_base+CR);
	writel(0x7FF,uart_base+ICR);
	
	writel(DIVISOR,uart_base+IBRD);
	writel(DIVISOR,uart_base+FBRB);
	// enabling fifo
	writel((1<<4)|(3<<5),uart_base+LCRH);

	write((1<<0)|(1<<8)|(1<<9),uart_base+CR);
}

static void ___exit uart_exit(void)
{
	writel(0x0,uart_base+CR);
	pr_info("UART0 driver unloaded\n");
}
