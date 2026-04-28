#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/ktime.h>
#include <linux/of.h>

#include "font8x8_basic.h"

#define SCREEN_WIDTH 176
#define SCREEN_HEIGHT 220

struct ultra_lcd {
    struct spi_device *spi;

    /* LCD GPIOs */
    struct gpio_desc *dc;
    struct gpio_desc *reset;

    /* Ultrasonic GPIOs */
    struct gpio_desc *trig;
    struct gpio_desc *echo;
    int irq;

    ktime_t echo_start;
    unsigned long distance_cm;
};

static struct ultra_lcd *gdev;

/* ---------------- SPI Helpers ---------------- */

static int write16(struct ultra_lcd *dev, u16 val)
{
    u8 buf[2] = { val >> 8, val & 0xFF };
    return spi_write(dev->spi, buf, 2);
}

static void write_reg(struct ultra_lcd *dev, u16 reg, u16 data)
{
    gpiod_set_value(dev->dc, 0);
    write16(dev, reg);
    gpiod_set_value(dev->dc, 1);
    write16(dev, data);
}

static void lcd_reset(struct ultra_lcd *dev)
{
    gpiod_set_value(dev->reset, 1);
    msleep(5);
    gpiod_set_value(dev->reset, 0);
    msleep(20);
    gpiod_set_value(dev->reset, 1);
    msleep(50);
}

static void lcd_init(struct ultra_lcd *dev)
{
    lcd_reset(dev);
    write_reg(dev, 0x0007, 0x1017);
    msleep(20);
}

static void drawPixel(int x, int y, u16 color)
{
    struct ultra_lcd *dev = gdev;

    gpiod_set_value(dev->dc, 0);
    write16(dev, 0x0020);
    gpiod_set_value(dev->dc, 1);
    write16(dev, x);

    gpiod_set_value(dev->dc, 0);
    write16(dev, 0x0021);
    gpiod_set_value(dev->dc, 1);
    write16(dev, y);

    gpiod_set_value(dev->dc, 0);
    write16(dev, 0x0022);
    gpiod_set_value(dev->dc, 1);
    write16(dev, color);
}

static void drawChar(int x, int y, char c, u16 color)
{
    if (c < 32 || c > 127)
        return;

    const u8 *bitmap = font8x8[c - 32];

    for (int r = 0; r < 8; r++) {
        for (int col = 0; col < 8; col++) {
            if (bitmap[r] & (1 << (7 - col))) {
                drawPixel(x + col * 2, y + r * 2, color);
                drawPixel(x + col * 2 + 1, y + r * 2, color);
                drawPixel(x + col * 2, y + r * 2 + 1, color);
                drawPixel(x + col * 2 + 1, y + r * 2 + 1, color);
            }
        }
    }
}

static void drawString(int x, int y, const char *str)
{
    while (*str) {
        drawChar(x, y, *str, 0x0000);
        x += 16;
        str++;
    }
}

/* ---------------- Ultrasonic ---------------- */

static void trigger_pulse(struct ultra_lcd *dev)
{
    gpiod_set_value(dev->trig, 0);
    udelay(2);
    gpiod_set_value(dev->trig, 1);
    udelay(10);
    gpiod_set_value(dev->trig, 0);
}

static irqreturn_t echo_irq(int irq, void *data)
{
    struct ultra_lcd *dev = data;

    if (gpiod_get_value(dev->echo)) {
        /* Rising edge */
        dev->echo_start = ktime_get();
    } else {
        /* Falling edge */
        ktime_t end = ktime_get();
        s64 duration_us;

        duration_us = ktime_to_us(ktime_sub(end, dev->echo_start));

        if (duration_us > 0)
            dev->distance_cm = duration_us / 58;

        /* Clear small area by overwriting text */
        {
            char buf[32];
            snprintf(buf, sizeof(buf), "Distance: %lu cm", dev->distance_cm);
            drawString(0, 0, buf);
        }

        /* Trigger next measurement */
        trigger_pulse(dev);
    }

    return IRQ_HANDLED;
}

/* ---------------- Probe ---------------- */

static int ultra_probe(struct spi_device *spi)
{
    struct ultra_lcd *dev;
    int ret;

    dev = devm_kzalloc(&spi->dev, sizeof(*dev), GFP_KERNEL);
    if (!dev)
        return -ENOMEM;

    dev->spi = spi;
    gdev = dev;

    /* LCD GPIOs */
    dev->dc = devm_gpiod_get(&spi->dev, "dc", GPIOD_OUT_LOW);
    dev->reset = devm_gpiod_get(&spi->dev, "reset", GPIOD_OUT_HIGH);

    /* Ultrasonic GPIOs */
    dev->trig = devm_gpiod_get(&spi->dev, "trig", GPIOD_OUT_LOW);
    dev->echo = devm_gpiod_get(&spi->dev, "echo", GPIOD_IN);

    if (IS_ERR(dev->dc) || IS_ERR(dev->reset) ||
        IS_ERR(dev->trig) || IS_ERR(dev->echo))
        return -EINVAL;

    spi_setup(spi);
    lcd_init(dev);

    /* Initial trigger */
    trigger_pulse(dev);

    dev->irq = gpiod_to_irq(dev->echo);
    ret = request_irq(dev->irq,
                      echo_irq,
                      IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                      "ultra_irq",
                      dev);
    if (ret)
        return ret;

    dev_info(&spi->dev, "Ultrasonic + LCD driver loaded\n");
    return 0;
}

static void ultra_remove(struct spi_device *spi)
{
    struct ultra_lcd *dev = gdev;
    free_irq(dev->irq, dev);
}

/* ---------------- Device Tree Match ---------------- */

static const struct of_device_id ultra_dt_ids[] = {
    { .compatible = "mycompany,ultra-lcd" },
    { }
};
MODULE_DEVICE_TABLE(of, ultra_dt_ids);

static struct spi_driver ultra_driver = {
    .driver = {
        .name = "ultra_lcd",
        .of_match_table = ultra_dt_ids,
    },
    .probe  = ultra_probe,
    .remove = ultra_remove,
};

module_spi_driver(ultra_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Integrated");
MODULE_DESCRIPTION("Ultrasonic Distance Display on ILI9225");
