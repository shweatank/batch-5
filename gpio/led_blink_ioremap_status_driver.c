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
#define GPFLEV0_OFFSET   0x34
static void __iomem *gpio_base;

static int __init led_init(void)
{
    u32 val,status,res;

    pr_info("LED ioremap driver loaded\n");

    gpio_base = ioremap(GPIO_BASE_PHYS, GPIO_SIZE);
    if (!gpio_base) {
        pr_err("ioremap failed\n");
        return -ENOMEM;
    }

    /* Set GPIO17,GPIO18 as OUTPUT */
    val = readl(gpio_base + GPFSEL1_OFFSET);
    status=readl(gpio_base+GPFSEL1_OFFSET);
    val &= ~(7 << 21);   // clear bits
    val |=  (1 << 21);   // set output
    status &=~(7<<24); //clear bits

    writel(val, gpio_base + GPFSEL1_OFFSET);
    writel(status,gpio_base + GPFSEL1_OFFSET);
     res=0;
   
     status=readl(gpio_base+GPFLEV0_OFFSET);

    res=((status>>18)&0x01);
    /* Blink LED 10 times */
    if(res==1)
    {
        /* LED ON */
        writel(1 << 17, gpio_base + GPSET0_OFFSET);
	pr_info("led on\n");
	msleep(500);
      }
    else
    {

        /* LED OFF */
        writel(1 << 17, gpio_base + GPCLR0_OFFSET);
	pr_info("led off\n");
       msleep(500);
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
