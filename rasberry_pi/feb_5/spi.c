#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/delay.h>

#define SPI0_BASE_PHYS   0xFE204000
#define SPI0_SIZE        0x18

#define SPI0_CS     0x00
#define SPI0_FIFO   0x04
#define SPI0_CLK    0x0B
#define SPI0_DLEN   0x0C
#define SPI0_LTOH   0x10
#define SPI0_DC     0x14

#define RXF		(1 << 20)
#define RXR		(1 << 19)
#define TXD		(1 << 18)
#define RXD		(1 << 17)
#define DONE		(1 << 16)
#define DMAEN		(1 << 8)
static void __iomem *spi_base;

static void spi_putc(char ch)
{
    unsigned int val;
    
    //disabling dmaen bit to enable polling
    val = readl(spi_base + SPI0_CS);
    val &= ~DMAEN;
    writel(val,spi_base + SPI0_CS);

    // wait until tx fifo has space
    while (!(readl(spi_base + SPI0_CS) & TXD));

    //write the data
    writel(ch,spi_base + SPI0_FIFO);

    //wait till the transfer complete
    while(!(readl(spi_base + SPI0_CS) & DONE));

    //clear the done bit
    writel(readl(spi_base + SPI0_CS) | DONE, spi_base + SPI0_CS);
}

static char spi_getc() 
{
    unsigned int cs;
    char ch;






static int __init spi_init(void)
{
    unsigned int val;
    
    pr_info("SPI0 ioremap driver init\n");

    spi_base = ioremap(SPI0_BASE_PHYS, SPI0_SIZE);
    if (!spi_base) {
        pr_err("SPI0 ioremap failed\n");
        return -ENOMEM;
    }

    //disable spi
    writel(0x0, spi_base + SPI0_CS);

    //clear FIFOs + pending interrupts
    val = (1 << 4) | (1 << 5);   // CLEAR_RX | CLEAR_TX
    writel(val, spi_base + SPI0_CS);

    //clear done,rxr,rxd,txd,rxf bits
    val |= RXF | TXD | RXD | RXR | DONE;
    writel(val,spi_base + SPI0_CS);

    //set clock divider (250Mhz/250 = 1Mhz)
    writel(250,spi_base + SPI0_CLK);

    //set the mode0(cpol = 0,cpha = 0)
    val = 0;
    val |= (0 << 3) | (0 << 2) | (0 << 0);
    writel(val,spi_base + SPI0_CS);

    //enable spi
    val |= (1 << 7); //setting TA bit
    writel(val,spi_base + SPI0_CS);

    pr_info("SPI0 initialized\n");
    return 0;
}

static void __exit spi_exit(void) 
{
    writel(0x0, spi_base + SPI0_CS);

    if (spi_base)
        iounmap(spi_base);

    pr_info("SPI0 ioremap driver unloaded\n");
}

module_init(spi_init);
module_exit(spi_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ajay");
MODULE_DESCRIPTION("BCM2711 SPI0 driver using ioremap only");
