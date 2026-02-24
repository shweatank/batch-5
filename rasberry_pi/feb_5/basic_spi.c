#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>

#define SPI0_BASE_PHYS   0xFE204000
#define SPI0_SIZE        0x100

#define SPI0_CS     0x00
#define SPI0_FIFO   0x04
#define SPI0_CLK    0x08
#define SPI0_DLEN   0x0C
#define SPI0_LTOH   0x10
#define SPI0_DC     0x14

#define RXF		(1 << 20)
#define RXR		(1 << 19)
#define TXD		(1 << 18)
#define RXD		(1 << 17)
#define DONE		(1 << 16)
#define DMAEN		(1 << 8)

#define GPFSEL1   0x04

#define GPIO_BASE	 0xFE200000
#define DEVICE_NAME	"spi_driver"

static dev_t devno;
static struct cdev spi_cdev;
static struct class *spi_class;

static void __iomem *spi_base;

static void spi_putc(char ch)
{
    unsigned int val;
    
    //disabling dmaen bit to enable polling
    val = readl(spi_base + SPI0_CS);
    val &= ~DMAEN;
    val |= (1 << 7);	//TA bit enabling
    writel(val,spi_base + SPI0_CS);

    //write the data
    writel(ch,spi_base + SPI0_FIFO);

    //wait till the transfer complete
    while(!(readl(spi_base + SPI0_CS) & DONE));

    //clear the done bit
    writel(readl(spi_base + SPI0_CS) | DONE, spi_base + SPI0_CS);
}

static int spi_getc(void)
{
    u32 cs;
    int timeout;

    /* Clear FIFOs */
    cs = readl(spi_base + SPI0_CS);
    cs |= (1 << 4) | (1 << 5);   // CLEAR RX + TX
    writel(cs, spi_base + SPI0_CS);

    /* Enable TA */
    cs = readl(spi_base + SPI0_CS);
    cs |= (1 << 7);   // TA
    writel(cs, spi_base + SPI0_CS);

    /* Read received byte */
    int val = readl(spi_base + SPI0_FIFO) & 0xFF;
    }
    /* Disable TA */
    cs = readl(spi_base + SPI0_CS);
    cs &= ~(1 << 7);
    writel(cs, spi_base + SPI0_CS);

    return val;
}


static ssize_t spi_write_user(struct file *file,const char __user *buf,size_t len,loff_t *off) {
	char kbuf[128];
	int i;
	if(copy_from_user(kbuf,buf,len))
		return -EFAULT;
	for(i = 0;i<len;i++) {
		spi_putc(kbuf[i]);
	}
	pr_info("Transmitted data: %s\n",kbuf);
	return len;
}

static ssize_t spi_read_user(struct file *file,
                             char __user *buf,
                             size_t len,
                             loff_t *offset)
{
    char kbuf[128];
    size_t i;
    int ret;

    /* If already read once, return EOF */
    if (*offset > 0)
        return 0;

    if (len > sizeof(kbuf))
        len = sizeof(kbuf);

    for (i = 0; i < len; i++) {
        ret = spi_getc();
	pr_info("received byte: %c\n",ret);
        if (ret < 0)
            return ret;
        kbuf[i] = ret;
    }
    if (copy_to_user(buf, kbuf, len))
        return -EFAULT;

    *offset += len;   // mark as read
    pr_info("Received data: %s\n",buf);
    return len;
}


static struct file_operations spi_fops = {
    .owner = THIS_MODULE,
    .read  = spi_read_user,
    .write = spi_write_user,
};

void spi_gpio_init(void)
{
    void __iomem *gpio;
    u32 val;

    gpio = ioremap(GPIO_BASE, 0x100);

    val = readl(gpio + GPFSEL1);

    /* Clear bits for GPIO9,10,11 */
    val &= ~((7 << 27) | (7 << 0) | (7 << 3));

    /* Set ALT0 (100) */
    val |=  (4 << 27);  // GPIO9
    val |=  (4 << 0);   // GPIO10
    val |=  (4 << 3);   // GPIO11

    writel(val, gpio + GPFSEL1);
}


static void spi_hw_init(void) {
    
    unsigned int val;

    spi_gpio_init();

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
    val &= ~((1 << 3) | (1 << 2));
    writel(val,spi_base + SPI0_CS);

    //enable spi
    val = readl(spi_base + SPI0_CS);
    val |= (1 << 7); //setting TA bit
    writel(val,spi_base + SPI0_CS);

}

static int __init spi_init(void)
{
    int ret;
    
    pr_info("SPI0 ioremap driver init\n");

    spi_base = ioremap(SPI0_BASE_PHYS, SPI0_SIZE);
    if (!spi_base) {
        pr_err("SPI0 ioremap failed\n");
        return -ENOMEM;
    }

    ret = alloc_chrdev_region(&devno,0,1,DEVICE_NAME);
    if(ret)
	    goto unmap;

    cdev_init(&spi_cdev,&spi_fops);
    ret = cdev_add(&spi_cdev,devno,1);
    if(ret)
	    goto unregister;

    spi_class = class_create(DEVICE_NAME);
    device_create(spi_class,NULL,devno,NULL,DEVICE_NAME);

    spi_hw_init();

    pr_info("spi_driver: loaded\n");
    return 0;

unregister:
    unregister_chrdev_region(devno,1);
unmap:
    iounmap(spi_base);
    return ret;
}

static void __exit spi_exit(void) 
{
    writel(0x0, spi_base + SPI0_CS);

    device_destroy(spi_class, devno);
    class_destroy(spi_class);
    cdev_del(&spi_cdev);
    unregister_chrdev_region(devno,1);

    if (spi_base)
        iounmap(spi_base);

    pr_info("SPI0 ioremap driver unloaded\n");
}

module_init(spi_init);
module_exit(spi_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ajay");
MODULE_DESCRIPTION("BCM2711 SPI0 driver using ioremap only");
