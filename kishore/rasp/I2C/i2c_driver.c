#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/clk.h>
#include <linux/delay.h>

#define CON     0x00
#define STAT    0x04
#define DLEN    0x08
#define SLAVE   0x0C
#define FIFO    0x10
#define DIV     0x14

#define DEVICE_NAME "my_i2c"
#define CLASS_NAME  "my_i2c_class"

struct my_i2c {
	void __iomem *i2c_base;
	struct cdev cdev;
	dev_t devt;
	struct class *class;
	struct device *device;
	struct clk *clk;
};

/* ---------------------------------------------------- */
/*                Low Level I2C Functions               */
/* ---------------------------------------------------- */

static void i2c_init(struct my_i2c *dev)
{
	u32 val;

	/* Disable controller */
	writel(0, dev->i2c_base + CON);

	/* Set clock divider */
	val = readl(dev->i2c_base + DIV);
	val &= ~0xFFFF;
	val |= 1500;
	writel(val, dev->i2c_base + DIV);

	/* Set slave address (example 0x3C) */
	writel(0x3C, dev->i2c_base + SLAVE);

	/* Clear status flags */
	writel((1<<1)|(1<<8)|(1<<9), dev->i2c_base + STAT);

	pr_info("I2C initialized\n");
}

static void i2c_write_byte(struct my_i2c *dev, char ch)
{
	u32 val;

	/* Clear status */
	writel((1<<1)|(1<<8)|(1<<9), dev->i2c_base + STAT);

	/* Set data length */
	writel(1, dev->i2c_base + DLEN);

	/* Enable I2C + Start transfer */
	val = readl(dev->i2c_base + CON);
	val |= (1<<7) | (1<<0);  /* I2CEN + ST */
	writel(val, dev->i2c_base + CON);

	/* Wait until TX FIFO can accept data */
	while (!(readl(dev->i2c_base + STAT) & (1<<4)))
		cpu_relax();

	writel(ch, dev->i2c_base + FIFO);

	/* Wait for transfer done */
	while (!(readl(dev->i2c_base + STAT) & (1<<1)))
		cpu_relax();

	/* Clear DONE flag */
	writel((1<<1), dev->i2c_base + STAT);

	pr_info("I2C wrote: %c\n", ch);
}

static char i2c_read_byte(struct my_i2c *dev)
{
	u32 val;
	char data;

	/* Clear status */
	writel((1<<1)|(1<<8)|(1<<9), dev->i2c_base + STAT);

	/* Set read length */
	writel(1, dev->i2c_base + DLEN);

	/* Enable I2C + Start + Read */
	val = readl(dev->i2c_base + CON);
	val |= (1<<7) | (1<<0) | (1<<4); /* I2CEN + ST + READ */
	writel(val, dev->i2c_base + CON);

	/* Wait until RX FIFO has data */
	while (!(readl(dev->i2c_base + STAT) & (1<<5)))
		cpu_relax();

	data = readl(dev->i2c_base + FIFO);

	/* Wait for DONE */
	while (!(readl(dev->i2c_base + STAT) & (1<<1)))
		cpu_relax();

	writel((1<<1), dev->i2c_base + STAT);

	pr_info("I2C read: %c\n", data);

	return data;
}

/* ---------------------------------------------------- */
/*               Character Driver Section               */
/* ---------------------------------------------------- */

static ssize_t my_write(struct file *file,
			const char __user *buf,
			size_t len,
			loff_t *off)
{
	struct my_i2c *dev = file->private_data;
	char kbuf[128];
	int i;

	if (len > sizeof(kbuf))
		len = sizeof(kbuf);

	if (copy_from_user(kbuf, buf, len))
		return -EFAULT;

	for (i = 0; i < len; i++)
		i2c_write_byte(dev, kbuf[i]);

	return len;
}

static ssize_t my_read(struct file *file,
		       char __user *buf,
		       size_t len,
		       loff_t *off)
{
	struct my_i2c *dev = file->private_data;
	char data;

	if (len == 0)
		return 0;

	data = i2c_read_byte(dev);

	if (copy_to_user(buf, &data, 1))
		return -EFAULT;

	return 1;
}

static int my_open(struct inode *inode, struct file *file)
{
	struct my_i2c *dev =
		container_of(inode->i_cdev, struct my_i2c, cdev);

	file->private_data = dev;
	return 0;
}

static const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.open  = my_open,
	.write = my_write,
	.read  = my_read,
};

/* ---------------------------------------------------- */
/*               Platform Driver Section                */
/* ---------------------------------------------------- */

static int my_probe(struct platform_device *pdev)
{
	struct my_i2c *dev;
	struct resource *res;
	int ret;

	pr_info("I2C probe called\n");

	dev = devm_kzalloc(&pdev->dev, sizeof(*dev), GFP_KERNEL);
	if (!dev)
		return -ENOMEM;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	dev->i2c_base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(dev->i2c_base))
		return PTR_ERR(dev->i2c_base);

	dev->clk = devm_clk_get(&pdev->dev, NULL);
	if (!IS_ERR(dev->clk))
		clk_prepare_enable(dev->clk);

	i2c_init(dev);

	ret = alloc_chrdev_region(&dev->devt, 0, 1, DEVICE_NAME);
	if (ret)
		return ret;

	cdev_init(&dev->cdev, &my_fops);
	ret = cdev_add(&dev->cdev, dev->devt, 1);
	if (ret)
		goto unregister_chrdev;

	dev->class = class_create("my_i2c_driver");
	dev->device = device_create(dev->class, NULL,
				    dev->devt, NULL,
				    DEVICE_NAME);

	platform_set_drvdata(pdev, dev);

	pr_info("I2C probe successful\n");
	return 0;

unregister_chrdev:
	unregister_chrdev_region(dev->devt, 1);
	return ret;
}

static void my_remove(struct platform_device *pdev)
{
	struct my_i2c *dev = platform_get_drvdata(pdev);

	device_destroy(dev->class, dev->devt);
	class_destroy(dev->class);
	cdev_del(&dev->cdev);
	unregister_chrdev_region(dev->devt, 1);

	if (!IS_ERR(dev->clk))
		clk_disable_unprepare(dev->clk);

	pr_info("I2C removed\n");
//	return 0;
}

static const struct of_device_id my_i2c_of_match[] = {
	{ .compatible = "my,i2c" },
	{ }
};
MODULE_DEVICE_TABLE(of, my_i2c_of_match);

static struct platform_driver my_i2c_driver = {
	.probe  = my_probe,
	.remove = my_remove,
	.driver = {
		.name           = "my_i2c",
		.of_match_table = my_i2c_of_match,
	},
};

module_platform_driver(my_i2c_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kishore");
MODULE_DESCRIPTION("Simple I2C Platform + Char Driver");

