// led_ioremap_driver.c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/timer.h>
#include <linux/delay.h>

#define GPIO_BASE_PHYS 0xFE200000  // Raspberry Pi 4 GPIO base
#define GPFSEL1  0x04              // Function select register for GPIO10-19
#define GPSET0   0x1C              // Set GPIO pins 0-31
#define GPCLR0   0x28              // Clear GPIO pins 0-31

#define GPIO_PIN17  17            // LED connected to GPIO17
#define BLINK_DELAY_MS 500         // Blink interval in milliseconds
#define GPIO_PIN18  18
static void __iomem *gpio_base;


// Module initialization
static int __init led_init(void)
{
    u32 val,temp;

    pr_info("LED ioremap driver init\n");

    // Map GPIO registers
    gpio_base = ioremap(GPIO_BASE_PHYS, 0xA0);
    if (!gpio_base) {
        pr_err("Failed to ioremap GPIO registers\n");
        return -ENOMEM;
    }

    // Set GPIO17 as output
    val = readl(gpio_base + GPFSEL1);        // Read current value
    val &= ~(7 << 21);                       // Clear bits 21-23 (GPIO17)
    val |= (1 << 21);                        // Set GPIO17 as output
    writel(val, gpio_base + GPFSEL1);

    temp=readl(gpio_base+GPFSEL1);
    val=(temp>>GPIO_PIN18)&0X01;


    

    if (val) {
        writel(1 << GPIO_PIN17, gpio_base + GPCLR0);
       	printk(KERN_INFO" led is on\n:");
    } else {
        writel(1 << GPIO_PIN17, gpio_base + GPSET0);
       	
       	printk(KERN_INFO" led is off\n:");
    }

    // Setup and start timer
 //   timer_setup(&blink_timer, blink_function, 0);
  //  mod_timer(&blink_timer, jiffies + msecs_to_jiffies(BLINK_DELAY_MS));
  

    return 0;
}

// Module cleanup
static void __exit led_exit(void)
{
  //  del_timer_sync(&blink_timer);            // Stop the timer
    writel(1 << GPIO_PIN17, gpio_base + GPCLR0); // Turn LED OFF
    iounmap(gpio_base);                       // Unmap GPIO registers

    pr_info("LED ioremap driver exit\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Techdhaba");
MODULE_DESCRIPTION("LED Blink Driver using ioremap");

