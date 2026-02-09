#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/delay.h>

#define SPI0_BASE_PHYS   0xFE204000
#define SPI0_SIZE        0x20

/* SPI0 Registers */
#define SPI_CS      0x00
#define SPI_FIFO    0x04
#define SPI_CLK     0x08
#define SPI_DLEN    0x0C
#define SPI_LTOH    0x10
#define SPI_DC      0x14

/* CS register bits */
#define CS_LEN_LONG     (1 << 25)
#define CS_DMA_LEN      (1 << 24)
#define CS_CSPOL2       (1 << 23)
#define CS_CSPOL1       (1 << 22)
#define CS_CSPOL0       (1 << 21)
#define CS_RXF          (1 << 20)
#define CS_RXR          (1 << 19)
#define CS_TXD          (1 << 18)
#define CS_RXD          (1 << 17)
#define CS_DONE         (1 << 16)
#define CS_TE_EN        (1 << 15)
#define CS_LMONO        (1 << 14)
#define CS_LEN          (1 << 13)
#define CS_REN          (1 << 12)
#define CS_ADCS         (1 << 11)
#define CS_INTR         (1 << 10)
#define CS_INTD         (1 << 9)
#define CS_DMAEN        (1 << 8)
#define CS_TA           (1 << 7)
#define CS_CSPOL        (1 << 6)
#define CS_CLEAR_RX     (1 << 5)
#define CS_CLEAR_TX     (1 << 4)
#define CS_CPOL         (1 << 3)
#define CS_CPHA         (1 << 2)
#define CS_CS_MASK      0x3

static void __iomem *spi_base;

/* Write one byte and read one byte */
static u8 spi_transfer(u8 data)
{
    /* Wait until TX FIFO can accept data */
    while (!(readl(spi_base + SPI_CS) & CS_TXD))
        cpu_relax();

    writel(data, spi_base + SPI_FIFO);

    /* Wait for RX data */
    while (!(readl(spi_base + SPI_CS) & CS_RXD))
        cpu_relax();

    return readl(spi_base + SPI_FIFO) & 0xFF;
}

static int __init spi_init(void)
{
    u32 cs;
    u8 rx;

    pr_info("SPI ioremap driver init\n");

    spi_base = ioremap(SPI0_BASE_PHYS, SPI0_SIZE);
    if (!spi_base) {
        pr_err("SPI ioremap failed\n");
        return -ENOMEM;
    }

    /* Disable SPI */
    writel(0, spi_base + SPI_CS);

    /* Clear FIFOs */
    writel(CS_CLEAR_RX | CS_CLEAR_TX, spi_base + SPI_CS);

    /*
     * SPI clock divider
     * Core freq = 250 MHz
     * Divider = 250MHz / 65536 ≈ 3.8 kHz (safe & slow)
     */
    writel(65536, spi_base + SPI_CLK);

    /* CS0, Mode 0 (CPOL=0, CPHA=0) */
    cs = readl(spi_base + SPI_CS);
    cs &= ~(CS_CS_MASK | CS_CPOL | CS_CPHA);
    writel(cs, spi_base + SPI_CS);

    /* Transfer active */
    writel(cs | CS_TA, spi_base + SPI_CS);

    pr_info("SPI transfer test\n");

    /* Example transfer */
    rx = spi_transfer(0x9F); /* JEDEC ID command (for flash) */

    pr_info("SPI received: 0x%02X\n", rx);

    /* End transfer */
    writel(readl(spi_base + SPI_CS) & ~CS_TA, spi_base + SPI_CS);

    return 0;
}

static void __exit spi_exit(void)
{
    /* Disable SPI */
    if (spi_base) {
        writel(0, spi_base + SPI_CS);
        iounmap(spi_base);
    }

    pr_info("SPI ioremap driver unloaded\n");
}

module_init(spi_init);
module_exit(spi_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prashant");
MODULE_DESCRIPTION("BCM2711 SPI0 driver using ioremap only");

