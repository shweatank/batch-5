#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/delay.h>

#define UART0_BASE_PHYS   0xFE201000
#define UART0_SIZE        0x90

/* UART Registers */
#define UART_DR     0x00
#define UART_FR     0x18
#define UART_IBRD   0x24
#define UART_FBRD   0x28
#define UART_LCRH   0x2C
#define UART_CR     0x30
#define UART_IMSC   0x38
#define UART_ICR    0x44

/* FR bits */
#define FR_TXFF     (1 << 5)
#define FR_RXFE     (1 << 4)

static void __iomem *uart_base;
static int __init uart_init(void)
{
    pr_info("UART ioremap driver init\n");

    uart_base = ioremap(UART0_BASE_PHYS, UART0_SIZE);
    if (!uart_base) {
        pr_err("UART ioremap failed\n");
        return -ENOMEM;
    }

    /* Disable UART */
    writel(0x0, uart_base + UART_CR);

    /* Clear interrupts */
    writel(0x7FF, uart_base + UART_ICR);
    writel(26, uart_base + UART_IBRD);
    writel(3,  uart_base + UART_FBRD);

    /* 8N1, FIFO enabled */
    writel((1 << 4) | (1 << 5) | (1 << 6),
           uart_base + UART_LCRH);

    /* Enable UART, TX, RX */
    writel((1 << 0) | (1 << 8),
           uart_base + UART_CR);
 while (readl(uart_base + UART_FR) & FR_TXFF)
        cpu_relax();

    writel(0x41, uart_base + UART_DR);
    pr_info("data transmitted");
    return 0;
}
static void __exit uart_exit(void)
{
	writel(0x0, uart_base + UART_CR);

    if (uart_base)
        iounmap(uart_base);

    pr_info("UART ioremap driver unloaded\n");
}
module_init(uart_init);
module_exit(uart_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prashant");
MODULE_DESCRIPTION("BCM2711 UART driver using ioremap only");
