// SPDX-License-Identifier: GPL-2.0
/*
 * Raspberry Pi 4B (BCM2711)
 * Hardcoded: GPIO17 input, GPIO27 output (LED)
 * Uses ioremap() for GPIO + System Timer (CLO) for delay.
 *
 * Trigger: Rising edge on GPIO17 -> blink GPIO27 5 times.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/types.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Pi4 hardcoded GPIO17 in -> blink GPIO27 out using System Timer delay (ioremap)");
MODULE_VERSION("1.0");

/* -------------------------
 * BCM2711 base addresses
 * -------------------------
 * RPi4 peripheral base: 0xFE000000
 * GPIO base offset:    0x200000 -> 0xFE200000
 * System Timer offset: 0x003000 -> 0xFE003000
 */
#define BCM2711_PERI_BASE        0xFE000000UL
#define GPIO_BASE_PHYS           (BCM2711_PERI_BASE + 0x200000UL)
#define SYSTMR_BASE_PHYS         (BCM2711_PERI_BASE + 0x003000UL)

#define GPIO_MAP_LEN             0xB4
#define SYSTMR_MAP_LEN           0x20

/* Hardcoded pins */
#define GPIO_IN                  17
#define GPIO_OUT                 27

/* Hardcoded blink behavior */
#define BLINK_COUNT              5
#define ON_MS                    150
#define OFF_MS                   150
#define POLL_MS                  2

/* GPIO register offsets */
#define GPFSEL0                  0x00
#define GPSET0                   0x1C
#define GPCLR0                   0x28
#define GPLEV0                   0x34

/* System Timer offsets */
#define SYST_CLO                 0x04    /* 1 MHz free-running counter */

static void __iomem *gpio_base;
static void __iomem *syst_base;
static struct task_struct *poll_thread;

/* ---- System Timer delay (busy wait) ---- */

static inline u32 syst_read_clo(void)
{
    return readl_relaxed(syst_base + SYST_CLO);
}

/* 1 tick = 1 us (System Timer runs at 1 MHz) */
static void syst_delay_us(u32 us)
{
    u32 start = syst_read_clo();
    while ((u32)(syst_read_clo() - start) < us)
        cpu_relax();
}

static void syst_delay_ms(u32 ms)
{
    while (ms--)
        syst_delay_us(1000);
}

/* ---- GPIO helpers ---- */

static void gpio_set_function(int gpio, int func /* 0=input, 1=output */)
{
    u32 reg   = GPFSEL0 + (gpio / 10) * 4;
    u32 shift = (gpio % 10) * 3;
    u32 val   = readl_relaxed(gpio_base + reg);

    val &= ~(0x7u << shift);
    val |= ((func & 0x7u) << shift);

    writel_relaxed(val, gpio_base + reg);
}

static inline void gpio_write(int gpio, bool high)
{
    u32 bank = gpio / 32;
    u32 bit  = gpio % 32;

    if (high)
        writel_relaxed(1u << bit, gpio_base + GPSET0 + bank * 4);
    else
        writel_relaxed(1u << bit, gpio_base + GPCLR0 + bank * 4);
}

static inline bool gpio_read(int gpio)
{
    u32 bank = gpio / 32;
    u32 bit  = gpio % 32;
    u32 v    = readl_relaxed(gpio_base + GPLEV0 + bank * 4);
    return (v & (1u << bit)) != 0;
}

static void blink_once(void)
{
    gpio_write(GPIO_OUT, true);
    syst_delay_ms(ON_MS);
    gpio_write(GPIO_OUT, false);
    syst_delay_ms(OFF_MS);
}

static void blink_n_times(int n)
{
    int i;
    for (i = 0; i < n; i++)
        blink_once();
}

/* ---- Polling thread ---- */

static int poll_fn(void *data)
{
    bool prev = false;

    pr_info("pi4: polling GPIO%d, blinking GPIO%d on rising edge\n", GPIO_IN, GPIO_OUT);

    while (!kthread_should_stop()) {
        bool cur = gpio_read(GPIO_IN);

        /* Rising edge trigger (LOW -> HIGH) */
        if (!prev && cur) {
            pr_info("pi4: GPIO%d rising edge detected -> blink %d times\n",
                    GPIO_IN, BLINK_COUNT);
            blink_n_times(BLINK_COUNT);
        }

        prev = cur;
        syst_delay_ms(POLL_MS);
    }

    pr_info("pi4: poll thread stopped\n");
    return 0;
}

/* ---- Module init/exit ---- */

static int __init pi4_init(void)
{
    gpio_base = ioremap(GPIO_BASE_PHYS, GPIO_MAP_LEN);
    if (!gpio_base) {
        pr_err("pi4: ioremap GPIO failed\n");
        return -ENOMEM;
    }

    syst_base = ioremap(SYSTMR_BASE_PHYS, SYSTMR_MAP_LEN);
    if (!syst_base) {
        pr_err("pi4: ioremap SYSTMR failed\n");
        iounmap(gpio_base);
        return -ENOMEM;
    }

    /* Configure pins */
    gpio_set_function(GPIO_IN, 0);   /* input */
    gpio_set_function(GPIO_OUT, 1);  /* output */
    gpio_write(GPIO_OUT, false);

    pr_info("pi4: mapped GPIO@0x%lx, SYSTMR@0x%lx\n",
            (unsigned long)GPIO_BASE_PHYS, (unsigned long)SYSTMR_BASE_PHYS);

    poll_thread = kthread_run(poll_fn, NULL, "pi4_gpio_poll");
    if (IS_ERR(poll_thread)) {
        pr_err("pi4: kthread_run failed\n");
        poll_thread = NULL;
        gpio_write(GPIO_OUT, false);
        iounmap(syst_base);
        iounmap(gpio_base);
        return -ECHILD;
    }

    pr_info("pi4: loaded (hardcoded). Input=GPIO%d Output=GPIO%d\n", GPIO_IN, GPIO_OUT);
    return 0;
}

static void __exit pi4_exit(void)
{
    if (poll_thread)
        kthread_stop(poll_thread);

    if (gpio_base)
        gpio_write(GPIO_OUT, false);

    if (syst_base)
        iounmap(syst_base);
    if (gpio_base)
        iounmap(gpio_base);

    pr_info("pi4: unloaded\n");
}

module_init(pi4_init);
module_exit(pi4_exit);
