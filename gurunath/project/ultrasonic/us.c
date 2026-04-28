#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "hcsr04_iomap"

/* GPIO base (Pi 4) */
#define GPIO_BASE_PHYS 0xFE200000
#define BLOCK_SIZE 4096

/* HC-SR04 pins */
#define TRIG 12
#define ECHO 13

static void __iomem *gpio_base;
static dev_t devno;
static struct cdev cdev;
static struct task_struct *thread;
static unsigned int distance_mm;

/* Helper macros */
#define GPIO_SET(reg)  (gpio_base + 0x1C + ((reg) / 32) * 4)
#define GPIO_CLR(reg)  (gpio_base + 0x28 + ((reg) / 32) * 4)
#define GPIO_LEV(reg)  (gpio_base + 0x34 + ((reg) / 32) * 4)
#define GPIO_FSEL(reg) (gpio_base + 0x00 + ((reg)/10)*4)
#define GPIO_FSEL_SHIFT(pin)  ((pin)%10*3)

/* Set GPIO function: 1=output, 0=input */
static void set_gpio_func(int pin, int func)
{
    u32 val;
    void __iomem *fsel = GPIO_FSEL(pin);

    val = readl(fsel);
    val &= ~(0x7 << GPIO_FSEL_SHIFT(pin));
    val |= (func & 0x7) << GPIO_FSEL_SHIFT(pin);
    writel(val, fsel);
}

/* Write GPIO */
static void gpio_write(int pin, int value)
{
    if (value)
        writel(1 << (pin % 32), GPIO_SET(pin));
    else
        writel(1 << (pin % 32), GPIO_CLR(pin));
}

/* Read GPIO */
static int gpio_read(int pin)
{
    u32 val = readl(GPIO_LEV(pin));
    return (val & (1 << (pin % 32))) ? 1 : 0;
}

/* Trigger pulse */
static void trigger_pulse(void)
{
    gpio_write(TRIG, 0);
    udelay(2);
    gpio_write(TRIG, 1);
    udelay(10);
    gpio_write(TRIG, 0);
}

/* Kernel thread for measuring distance */
static int measure_fn(void *data)
{
    while (!kthread_should_stop()) {
        ktime_t start, end;
        s64 duration_ns;
        unsigned int value;

        trigger_pulse();

        while (!gpio_read(ECHO))
            cpu_relax();
        start = ktime_get();

        while (gpio_read(ECHO))
            cpu_relax();
        end = ktime_get();

        duration_ns = ktime_to_ns(ktime_sub(end, start));
        value = (unsigned int)((duration_ns * 343000ULL) / (2ULL * 1000000000ULL));
        distance_mm = value;

        msleep(100);
    }
    return 0;
}

/* read() */
static ssize_t hcsr04_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
    char kbuf[32];
    int bytes;

    if (*offset > 0)
        return 0;

    bytes = snprintf(kbuf, sizeof(kbuf), "%u.%u cm\n", distance_mm/10, distance_mm%10);
    if (len < bytes)
        return -EINVAL;

    if (copy_to_user(buf, kbuf, bytes))
        return -EFAULT;

    *offset += bytes;
    return bytes;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = hcsr04_read,
};

/* Module init */
static int __init ultrasonic_init(void)
{
    int ret;

    printk(KERN_INFO "HC-SR04 driver using ioremap GPIO\n");

    /* Map GPIO registers */
    gpio_base = ioremap(GPIO_BASE_PHYS, BLOCK_SIZE);
    if (!gpio_base) {
        printk(KERN_ERR "Failed to ioremap GPIO\n");
        return -ENOMEM;
    }

    /* Configure pins */
    set_gpio_func(TRIG, 1); // output
    set_gpio_func(ECHO, 0); // input

    /* Allocate char device */
    ret = alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME);
    if (ret)
        goto err_iounmap;

    cdev_init(&cdev, &fops);
    ret = cdev_add(&cdev, devno, 1);
    if (ret)
        goto err_region;

    /* Start measurement thread */
    thread = kthread_run(measure_fn, NULL, "hcsr04_iomap");
    if (IS_ERR(thread)) {
        ret = PTR_ERR(thread);
        goto err_cdev;
    }

    printk(KERN_INFO "Driver loaded. Create device node manually:\n");
    printk(KERN_INFO "sudo mknod /dev/hcsr04 c %d 0\n", MAJOR(devno));
    printk(KERN_INFO "sudo chmod 666 /dev/hcsr04\n");

    return 0;

err_cdev:
    cdev_del(&cdev);
err_region:
    unregister_chrdev_region(devno,1);
err_iounmap:
    iounmap(gpio_base);
    return ret;
}

/* Module exit */
static void __exit ultrasonic_exit(void)
{
    if (thread)
        kthread_stop(thread);

    cdev_del(&cdev);
    unregister_chrdev_region(devno,1);

    if (gpio_base)
        iounmap(gpio_base);

    printk(KERN_INFO "HC-SR04 ioremap driver unloaded\n");
}

module_init(ultrasonic_init);
module_exit(ultrasonic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("HC-SR04 kernel driver using ioremap to access GPIOs directly");

