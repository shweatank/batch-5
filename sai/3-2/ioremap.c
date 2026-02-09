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
u32 val,val2;
static void __iomem *gpio_base;
static void __iomem *gpio_base_2;

static int __init led_init(void)
{
    
    int i;

    pr_info("LED ioremap driver loaded\n");

    gpio_base = ioremap(GPIO_BASE_PHYS, GPIO_SIZE);
    gpio_base_2 = ioremap(GPIO_BASE_PHYS, GPIO_SIZE);
    if (!gpio_base) {
        pr_err("ioremap failed\n");
        return -ENOMEM;
    }

    /* Set GPIO17 as OUTPUT */
    val = readl(gpio_base + GPFSEL1_OFFSET);
    val &= ~(7 << 21);   // clear bits
    val |=  (1 << 21);   // set output
    writel(val, gpio_base + GPFSEL1_OFFSET);
    
    val2 = readl(gpio_base_2 + 0x08);
    val2 &= ~(7 << 21);   // clear bits
    //val2 |=  (0 << 21);   // set output
    writel(val2, gpio_base_2 + 0x08);

    /* Blink LED 10 times */
    for (i = 0; i < 10; i++) {
        /* LED ON */
	    if((val2>>21)&1)
	    {
        writel(1 << 17, gpio_base + GPSET0_OFFSET);
	printk("high\n");
        msleep(500);

        /* LED OFF */
        writel(1 << 17, gpio_base + GPCLR0_OFFSET);
	printk("off\n");
        msleep(500);
	    }
    }

   // set output
    

    return 0;
}

static void __exit led_exit(void)
{
    /* Ensure LED OFF */
	
    //val &= ~(1 << 21);   // set output
    writel(1 << 17, gpio_base + GPCLR0_OFFSET);
    if (gpio_base)
    {
        iounmap(gpio_base);
    }
    pr_info("LED ioremap driver unloaded\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prashant");
MODULE_DESCRIPTION("Raspberry Pi LED driver using ioremap only");
