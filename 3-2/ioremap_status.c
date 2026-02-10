#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/delay.h>

#define GPIO_BASE_PHYS  0xFE200000   // BCM2711 GPIO base
#define GPIO_SIZE      0xB4

#define GPFSEL1_OFFSET  0x04
#define GPSET0_OFFSET   0x1C
#define GPCLR0_OFFSET   0x28
#define GPFSEL2_OFFSET   0x08
#define GPFLEV0_OFFSET   0x34
static void __iomem *gpio_base;

static int __init led_init(void)
{
    u32 val17,val27;
    int i;

    pr_info("LED ioremap driver loaded\n");

    gpio_base = ioremap(GPIO_BASE_PHYS, GPIO_SIZE);
    if (!gpio_base) {
        pr_err("ioremap failed\n");
        return -ENOMEM;
    }

    /* Set GPIO17 as OUTPUT */
    val17 = readl(gpio_base + GPFSEL1_OFFSET);
    val17 &= ~(7 << 21);   // clear bits
    val17 |=  (1 << 21);   // set output
    writel(val17, gpio_base + GPFSEL1_OFFSET);
	val27=readl(gpio_base+GPFSEL2_OFFSET);
	val27&=~(7<<21);
    writel(val27, gpio_base + GPFSEL2_OFFSET);
    /* Blink LED 10 times */
    for (i = 0; i < 10; i++) {
        /* LED ON */
	val27=readl(gpio_base+GPFLEV0_OFFSET);
	    pr_info("val27=%d\n",((val27>>27)&1));
	    if(((val27>>27)&1)==1)
	    {
        writel(1 << 17, gpio_base + GPSET0_OFFSET);
	pr_info("LED ON\n");
        msleep(500);
        }
	    else
	    {
        /* LED OFF */
        writel(1 << 17, gpio_base + GPCLR0_OFFSET);
	pr_info("LED OFF\n");
        msleep(500);
	    }
    }

    return 0;
}

static void __exit led_exit(void)
{
    /* Ensure LED OFF */
    writel(1 << 17, gpio_base + GPCLR0_OFFSET);

    if (gpio_base)
        iounmap(gpio_base);

    pr_info("LED ioremap driver unloaded\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prashant");
MODULE_DESCRIPTION("Raspberry Pi LED driver using ioremap only");
