#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

#define SPI_BASE_ADDR  0xFE204000   /* Example: RPi SPI0 */
#define SPI_SIZE       0x100

#define CS    0x00
#define FIFO  0x04
#define CLK   0x08
#define DLEN  0x0C

#define DEVICE_NAME "spi_data"

static int major;
static void __iomem *base;
static DEFINE_MUTEX(spi_lock);

/* ---------- SPI INIT ---------- */
static void spi_init_hw(void)
{
	u32 cs;

	/* Disable SPI */
	writel(0x0, base + CS);

	/* Clear FIFOs */
	writel((1 << 4) | (1 << 5), base + CS);

	/* CPOL = 1, CPHA = 1, CS = 0 */
	cs = readl(base + CS);
	cs |= (1 << 3);          /* CPOL */
	cs |= (1 << 2);          /* CPHA */
	cs &= ~(3 << 4);         /* CS = 0 */
	writel(cs, base + CS);

	/* Clock divider */
	writel(100, base + CLK);

	pr_info("SPI initialized\n");
}

/* ---------- WRITE ---------- */
static ssize_t spi_write(struct file *file,
                         const char __user *buf,
                         size_t len,
                         loff_t *off)
{
	u8 ch;
	size_t i;
	u32 cs;
	int timeout;

	mutex_lock(&spi_lock);

	/* Set transfer length */
	writel(len, base + DLEN);

	/* Enable transfer (TA) */
	cs = readl(base + CS);
	cs |= (1 << 7);          /* TA */
	writel(cs, base + CS);

	for (i = 0; i < len; i++) {

		if (copy_from_user(&ch, &buf[i], 1)) {
			mutex_unlock(&spi_lock);
			return -EFAULT;
		}

		/* Wait until TX FIFO can accept data */
		timeout = 100000;
		while (!(readl(base + CS) & (1 << 18))) {
			if (--timeout == 0) {
				mutex_unlock(&spi_lock);
				return -ETIMEDOUT;
			}
			cpu_relax();
		}

		writel(ch, base + FIFO);
	}

	/* Wait for DONE */
	timeout = 100000;
	while (!(readl(base + CS) & (1 << 16))) {
		if (--timeout == 0) {
			mutex_unlock(&spi_lock);
			return -ETIMEDOUT;
		}
		cpu_relax();
	}

	/* Disable transfer */
	cs = readl(base + CS);
	cs &= ~(1 << 7);
	writel(cs, base + CS);

	mutex_unlock(&spi_lock);
	return len;
}

/* ---------- READ ---------- */
static ssize_t spi_read(struct file *file,
                        char __user *buf,
                        size_t len,
                        loff_t *off)
{
	u8 ch;
	size_t i;
	u32 cs;
	int timeout;

	mutex_lock(&spi_lock);

	writel(len, base + DLEN);

	cs = readl(base + CS);
	cs |= (1 << 7);      /* TA */
	writel(cs, base + CS);

	for (i = 0; i < len; i++) {

		/* Wait until RX FIFO has data */
		timeout = 100000;
		while (!(readl(base + CS) & (1 << 17))) {
			if (--timeout == 0) {
				mutex_unlock(&spi_lock);
				return -ETIMEDOUT;
			}
			cpu_relax();
		}

		ch = readl(base + FIFO) & 0xFF;

		if (copy_to_user(&buf[i], &ch, 1)) {
			mutex_unlock(&spi_lock);
			return -EFAULT;
		}
	}

	cs = readl(base + CS);
	cs &= ~(1 << 7);
	writel(cs, base + CS);

	mutex_unlock(&spi_lock);
	return len;
}

/* ---------- FILE OPS ---------- */
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read  = spi_read,
	.write = spi_write,
};

/* ---------- INIT ---------- */
static int __init spi_driver_init(void)
{
	base = ioremap(SPI_BASE_ADDR, SPI_SIZE);
	if (!base)
		return -ENOMEM;

	major = register_chrdev(0, DEVICE_NAME, &fops);
	if (major < 0) {
		iounmap(base);
		return major;
	}

	spi_init_hw();

	pr_info("SPI driver loaded (major=%d)\n", major);
	return 0;
}

/* ---------- EXIT ---------- */
static void __exit spi_driver_exit(void)
{
	unregister_chrdev(major, DEVICE_NAME);
	iounmap(base);
	pr_info("SPI driver unloaded\n");
}

module_init(spi_driver_init);
module_exit(spi_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KISHORE");
MODULE_DESCRIPTION("Corrected Basic SPI Character Driver");

