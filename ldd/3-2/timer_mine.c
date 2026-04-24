#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/delay.h>

#define GPIO_BASE_PHYS  0xFE200000   // BCM2711 GPIO base
#define GPIO_SIZE      0xB4

#define TIMER_PHY 0xFE003000
#define TIMER_CLO  0x04
#define TIMER_SIZE 0x1C

#define GPFSEL1_OFFSET  0x04
#define GPSET0_OFFSET   0x1C
#define GPCLR0_OFFSET   0x28
#define GPFSEL2_OFFSET   0x08
#define GPFLEV0_OFFSET   0x34
static void __iomem *gpio_base;
static void __iomem *timer_base;
static void delay_us(unsigned int us)
{
u32 start,now;
start=readl(timer_base+TIMER_CLO);
while(1)
{
now=readl(timer_base+TIMER_CLO);
if((now-start)>=us)
	break;
}
}
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
    timer_base=ioremap(TIMER_PHY,TIMER_SIZE);
    if (!timer_base) {
        pr_err("ioremap failed\n");
        return -ENOMEM;
    }

    /* Set GPIO17 as OUTPUT */
    val17 = readl(gpio_base + GPFSEL1_OFFSET);
    val17 &= ~(7 << 21);   // clear bits
    val17 |=  (1 << 21);   // set output
    writel(val17, gpio_base + GPFSEL1_OFFSET);
    /* Blink LED 10 times */
    for (i = 0; i < 10; i++) {
        /* LED ON */
        writel(1 << 17, gpio_base + GPSET0_OFFSET);
	pr_info("LED ON\n");
        delay_us(500000);
        /* LED OFF */
        writel(1 << 17, gpio_base + GPCLR0_OFFSET);
	pr_info("LED OFF\n");
        delay_us(500000);
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
