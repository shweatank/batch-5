#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/gpio/consumer.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/io.h>
#include "font8x8_basic.h"

/* LCD Constants */
#define DRIVER_NAME "ili9225_combined"
#define CLASS_NAME  "ili"
#define DEVICE_NAME "ili9225_char"
#define LCD_WIDTH   220
#define LCD_HEIGHT  176

/* DHT11 & GPIO Constants */
#define DHT_GPIO_4 4+512
#define GPIO_17    17+512
#define TIMER_PHY  0xFE003000
#define TIMER_CLO  0x04
#define TIMER_SIZE 0x1C
#define MSEC       1000

/* Structures */
struct dht_data {
    int temperature;
    int humidity;
};

struct ili9225 {
    struct spi_device *spi;
    struct gpio_desc *rs;
    struct gpio_desc *reset;
};

/* Global Variables */
static struct ili9225 *g_lcd;
static dev_t dev_num;
static struct class *ili_class;
static struct cdev ili_cdev;
static struct dht_data *sensor_data;
static int irq;
static struct workqueue_struct *my_wq;
static struct work_struct my_work;
static void __iomem *timer_base;

/* --- TIMER & SENSOR LOGIC --- */

static void delay_us(unsigned int us) {
    u32 start, now;
    start = readl(timer_base + TIMER_CLO);
    while (1) {
        now = readl(timer_base + TIMER_CLO);
        if ((now - start) >= us) break;
    }
}

/* Forward declarations for LCD functions used in sensor_work */
static void ili9225_fill(struct ili9225 *lcd, u16 color);
static void drawString(int x, int y, const char *str, uint16_t color);

static void sensor_work(struct work_struct *work) {
    int i, j;
    u8 buf[5] = {0};
    char temp_str[32], humid_str[32];

    /* Start signal */
    gpio_direction_output(DHT_GPIO_4, 0);
    delay_us(18 * MSEC);
    gpio_set_value(DHT_GPIO_4, 1);
    delay_us(30);
    gpio_direction_input(DHT_GPIO_4);

    /* Response & Read (Simplified for brevity, matches your logic) */
    while (!gpio_get_value(DHT_GPIO_4));
    while (gpio_get_value(DHT_GPIO_4));

    for (j = 0; j < 5; j++) {
        for (i = 0; i < 8; i++) {
            while (!gpio_get_value(DHT_GPIO_4));
            delay_us(30);
            if (gpio_get_value(DHT_GPIO_4)) buf[j] |= (1 << (7 - i));
            while (gpio_get_value(DHT_GPIO_4));
        }
    }

    if (((buf[0] + buf[1] + buf[2] + buf[3]) & 0xFF) == buf[4]) {
        sensor_data->humidity = buf[0];
        sensor_data->temperature = buf[2];

        /* UPDATE LCD IMMEDIATELY */
        if (g_lcd) {
            ili9225_fill(g_lcd, 0xFFFF);
            snprintf(temp_str, sizeof(temp_str), "Temp: %d C", sensor_data->temperature);
            snprintf(humid_str, sizeof(humid_str), "Humid: %d%%", sensor_data->humidity);
            drawString(0, 0, temp_str, 0xF800);  // Red Text
            drawString(0, 26, humid_str, 0x001F); // Blue Text
        }
    }
    enable_irq(irq);
}

static irqreturn_t GPIO_isr(int irq, void *dev_id) {
    disable_irq_nosync(irq);
    queue_work(my_wq, &my_work);
    return IRQ_HANDLED;
}

/* --- LCD CORE LOGIC (3x3 Scaling & Corner Rotation) --- */

static int ili9225_write16(struct ili9225 *lcd, u16 value) {
    u8 buf[2];
    buf[0] = value >> 8;
    buf[1] = value & 0xFF;
    return spi_write(lcd->spi, buf, 2);
}

static int ili9225_write_reg(struct ili9225 *lcd, u16 reg, u16 data) {
    gpiod_set_value(lcd->rs, 0);
    ili9225_write16(lcd, reg);
    gpiod_set_value(lcd->rs, 1);
    return ili9225_write16(lcd, data);
}

static void drawPixel(int x, int y, uint16_t color) {
    int x_rot = (LCD_WIDTH - 1) - x;
    int y_rot = y;
    ili9225_write_reg(g_lcd, 0x0020, y_rot);
    ili9225_write_reg(g_lcd, 0x0021, x_rot);
    gpiod_set_value(g_lcd->rs, 0);
    ili9225_write16(g_lcd, 0x0022);
    gpiod_set_value(g_lcd->rs, 1);
    ili9225_write16(g_lcd, color);
}

static void drawChar(int x, int y, char c, uint16_t color) {
    if (c < 32 || c > 127) return;
    const uint8_t *bitmap = font8x8[c - 32];
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (bitmap[row] & (1 << (7 - col))) {
                for (int dy = 0; dy < 3; dy++) {
                    for (int dx = 0; dx < 3; dx++) {
                        drawPixel(x + (col * 3) + dx, y + (row * 3) + dy, color);
                    }
                }
            }
        }
    }
}

static void drawString(int x, int y, const char *str, uint16_t color) {
    for (int i = 0; str[i]; i++) {
        drawChar(x, y, str[i], color);
        x += 25; // Spacing for 3x3 font
    }
}

static void ili9225_fill(struct ili9225 *lcd, u16 color) {
    int i;
    ili9225_write_reg(lcd, 0x0036, LCD_HEIGHT - 1);
    ili9225_write_reg(lcd, 0x0037, 0);
    ili9225_write_reg(lcd, 0x0038, LCD_WIDTH - 1);
    ili9225_write_reg(lcd, 0x0039, 0);
    ili9225_write_reg(lcd, 0x0020, 0);
    ili9225_write_reg(lcd, 0x0021, 0);
    gpiod_set_value(lcd->rs, 0);
    ili9225_write16(lcd, 0x0022);
    gpiod_set_value(lcd->rs, 1);
    for (i = 0; i < (LCD_WIDTH * LCD_HEIGHT); i++)
        ili9225_write16(lcd, color);
}

static void ili9225_init(struct ili9225 *lcd) {
    gpiod_set_value(lcd->reset, 1); msleep(5);
    gpiod_set_value(lcd->reset, 0); msleep(20);
    gpiod_set_value(lcd->reset, 1); msleep(50);
    ili9225_write_reg(lcd, 0x0001, 0x011C);
    ili9225_write_reg(lcd, 0x0002, 0x0100);
    ili9225_write_reg(lcd, 0x0003, 0x1038); // Landscape
    ili9225_write_reg(lcd, 0x0007, 0x1017); msleep(20);
}

/* --- FILE OPERATIONS & PROBE --- */

static ssize_t ili_write(struct file *file, const char __user *buf, size_t len, loff_t *off) {
    char kbuf[64];
    if (len > 63) len = 63;
    if (copy_from_user(kbuf, buf, len)) return -EFAULT;
    kbuf[len] = '\0';
    ili9225_fill(g_lcd, 0xFFFF);
    drawString(0, 0, kbuf, 0x0000);
    return len;
}

static struct file_operations fops = { .owner = THIS_MODULE, .write = ili_write };

static int ili9225_probe(struct spi_device *spi) {
    int ret;
    g_lcd = devm_kzalloc(&spi->dev, sizeof(*g_lcd), GFP_KERNEL);
    if (!g_lcd) return -ENOMEM;
    g_lcd->spi = spi;
    g_lcd->rs = devm_gpiod_get(&spi->dev, "rs", GPIOD_OUT_LOW);
    g_lcd->reset = devm_gpiod_get(&spi->dev, "reset", GPIOD_OUT_HIGH);
    spi->mode = SPI_MODE_0;
    spi->bits_per_word = 8;
    spi_setup(spi);
    ili9225_init(g_lcd);
    ili9225_fill(g_lcd, 0xFFFF);
    alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    cdev_init(&ili_cdev, &fops);
    cdev_add(&ili_cdev, dev_num, 1);
    ili_class = class_create(CLASS_NAME);
    device_create(ili_class, NULL, dev_num, NULL, DEVICE_NAME);
    
    /* Timer & Sensor Init from GPIO Code */
    timer_base = ioremap(TIMER_PHY, TIMER_SIZE);
    gpio_request(GPIO_17, "button");
    gpio_direction_input(GPIO_17);
    irq = gpio_to_irq(GPIO_17);
    request_irq(irq, GPIO_isr, IRQF_TRIGGER_RISING, "GPIO_ISR", NULL);
    gpio_request(DHT_GPIO_4, "dht_pin");
    my_wq = create_singlethread_workqueue("dht_wq");
    INIT_WORK(&my_work, sensor_work);
    sensor_data = kmalloc(sizeof(*sensor_data), GFP_KERNEL);
    
    pr_info("Combined Driver Loaded\n");
    return 0;
}

static void ili9225_remove(struct spi_device *spi) {
    free_irq(irq, NULL);
    gpio_free(GPIO_17);
    gpio_free(DHT_GPIO_4);
    flush_workqueue(my_wq);
    destroy_workqueue(my_wq);
    kfree(sensor_data);
    iounmap(timer_base);
    device_destroy(ili_class, dev_num);
    class_destroy(ili_class);
    cdev_del(&ili_cdev);
    unregister_chrdev_region(dev_num, 1);
}

static const struct of_device_id ili9225_dt_ids[] = { { .compatible = "ilitek,ili9225" }, { } };
MODULE_DEVICE_TABLE(of, ili9225_dt_ids);

static struct spi_driver ili9225_driver = {
    .driver = { .name = DRIVER_NAME, .of_match_table = ili9225_dt_ids },
    .probe = ili9225_probe,
    .remove = ili9225_remove,
};

module_spi_driver(ili9225_driver);
MODULE_LICENSE("GPL");
