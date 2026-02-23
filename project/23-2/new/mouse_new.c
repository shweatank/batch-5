#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spi/spi.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include "font8x8_basic.h"

#define DRIVER_NAME "ili9225"
#define DEVICE_NAME "ili9225_char"
#define CLASS_NAME  "ili"

#define SCREEN_WIDTH 176
#define SCREEN_HEIGHT 220
#define MAX_STRING_LEN 128

/* LED MMIO */
#define LED_PHYS_ADDR  0xFE200000
#define GPFSEL1 0x04
#define GPSET0  0x1c
#define GPCLR0  0x28
#define LED_GPIO 17

/* ---------------- Globals ---------------- */
static dev_t dev_num;
static struct class *ili_class;
static struct cdev ili_cdev;

static struct ili9225 *g_lcd;
static void __iomem *gpio_base;

static struct workqueue_struct *lcd_wq;
static struct work_struct lcd_work;

static char stored_string[MAX_STRING_LEN];
static int cursor_x = 0, cursor_y = 0;
static bool quadrant_mode = false;

/* ---------------- ILI9225 struct ---------------- */
struct ili9225 {
    struct spi_device *spi;
    struct gpio_desc *dc;
    struct gpio_desc *reset;
};

/* ---------------- SPI helpers ---------------- */
static int ili9225_write16(struct ili9225 *lcd, u16 value)
{
    u8 buf[2] = { value >> 8, value & 0xFF };
    return spi_write(lcd->spi, buf, 2);
}

static int ili9225_write_reg(struct ili9225 *lcd, u16 reg, u16 data)
{
    gpiod_set_value(lcd->dc, 0);
    ili9225_write16(lcd, reg);
    gpiod_set_value(lcd->dc, 1);
    return ili9225_write16(lcd, data);
}

static void ili9225_reset(struct ili9225 *lcd)
{
    gpiod_set_value(lcd->reset, 1);
    msleep(5);
    gpiod_set_value(lcd->reset, 0);
    msleep(20);
    gpiod_set_value(lcd->reset, 1);
    msleep(50);
}

static void ili9225_init(struct ili9225 *lcd)
{
    ili9225_reset(lcd);
    ili9225_write_reg(lcd, 0x0007, 0x1017);
}

static void ili9225_fill(struct ili9225 *lcd, u16 color)
{
    int x, y;

    ili9225_write_reg(lcd, 0x0020, 0);
    ili9225_write_reg(lcd, 0x0021, 0);

    gpiod_set_value(lcd->dc, 0);
    ili9225_write16(lcd, 0x0022);
    gpiod_set_value(lcd->dc, 1);

    for(y = 0; y < SCREEN_HEIGHT; y++)
        for(x = 0; x < SCREEN_WIDTH; x++)
            ili9225_write16(lcd, color);
}

/* ---------------- Pixel / Character ---------------- */
static void drawPixel(int x, int y, uint16_t color)
{
    struct ili9225 *lcd = g_lcd;

    gpiod_set_value(lcd->dc, 0);
    ili9225_write16(lcd, 0x0020);
    gpiod_set_value(lcd->dc, 1);
    ili9225_write16(lcd, x);

    gpiod_set_value(lcd->dc, 0);
    ili9225_write16(lcd, 0x0021);
    gpiod_set_value(lcd->dc, 1);
    ili9225_write16(lcd, y);

    gpiod_set_value(lcd->dc, 0);
    ili9225_write16(lcd, 0x0022);
    gpiod_set_value(lcd->dc, 1);
    ili9225_write16(lcd, color);
}

static void drawChar(int x, int y, char c, uint16_t color)
{
    if(c < 32 || c > 127) return;
    const uint8_t *bitmap = font8x8[c - 32];

    for(int row = 0; row < 8; row++)
        for(int col = 0; col < 8; col++)
            if(bitmap[row] & (1 << (7 - col))) {
                drawPixel(x + col*2, y + row*2, color);
                drawPixel(x + col*2+1, y + row*2, color);
                drawPixel(x + col*2, y + row*2+1, color);
                drawPixel(x + col*2+1, y + row*2+1, color);
            }
}

/* ---------------- LCD workqueue ---------------- */
static void lcd_work_func(struct work_struct *work)
{
    ili9225_fill(g_lcd, 0xFFFF);
    cursor_x = cursor_y = 0;

    for(int i=0; stored_string[i]; i++){
        drawChar(cursor_x, cursor_y, stored_string[i], 0x0000);
        cursor_x += 16;
        if(cursor_x > SCREEN_WIDTH-16){
            cursor_x = 0;
            cursor_y += 16;
        }
    }
}

/* ---------------- LED control ---------------- */
static void gpio17_set_output(void)
{
    u32 val = ioread32(gpio_base + GPFSEL1);
    val &= ~(0x7 << 21);
    val |= (0x1 << 21);
    iowrite32(val, gpio_base + GPFSEL1);
}

static void led_on(void){ iowrite32(1<<LED_GPIO, gpio_base + GPSET0); }
static void led_off(void){ iowrite32(1<<LED_GPIO, gpio_base + GPCLR0); }

/* ---------------- Chardev ---------------- */
static ssize_t ili_write(struct file *file,
                         const char __user *buf,
                         size_t len,
                         loff_t *off)
{
    char kbuf[MAX_STRING_LEN];
    if(len > MAX_STRING_LEN-1) len = MAX_STRING_LEN-1;

    if(copy_from_user(kbuf, buf, len)) return -EFAULT;
    kbuf[len] = '\0';

    for(size_t i=0; i<len; i++){
        char c = kbuf[i];
        if(c == 0x03){ // Ctrl+C
            quadrant_mode = true;
            pr_info("Quadrant ENABLED\n");
            continue;
        }
        if(c == 0x1A){ // Ctrl+Z
            quadrant_mode = false;
            pr_info("Quadrant DISABLED\n");
            continue;
        }
    }

    /* Store the string for mouse actions */
    strncpy(stored_string, kbuf, MAX_STRING_LEN-1);
    stored_string[MAX_STRING_LEN-1] = '\0';

    pr_info("Stored string: %s\n", stored_string);
    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = ili_write,
};

/* ---------------- Mouse ---------------- */
static int x_pos = SCREEN_WIDTH/2;
static int y_pos = SCREEN_HEIGHT/2;

static void mouse_event(struct input_handle *handle,
                        unsigned int type,
                        unsigned int code,
                        int value)
{
    if(type == EV_REL){
        if(code == REL_X) x_pos += value;
        if(code == REL_Y) y_pos += value;
        if(x_pos < 0) x_pos = 0; if(x_pos > SCREEN_WIDTH) x_pos = SCREEN_WIDTH;
        if(y_pos < 0) y_pos = 0; if(y_pos > SCREEN_HEIGHT) y_pos = SCREEN_HEIGHT;
    }

    if(!quadrant_mode) return;

    if(type == EV_KEY && code == BTN_LEFT && value == 1){
        if(x_pos < SCREEN_WIDTH/2 && y_pos < SCREEN_HEIGHT/2){
            pr_info("TOP LEFT → Send string via chardev\n");
            pr_info("%s\n", stored_string); // or send via UART if implemented
        } else if(x_pos > SCREEN_WIDTH/2 && y_pos > SCREEN_HEIGHT/2){
            pr_info("BOTTOM RIGHT → Display on LCD\n");
            queue_work(lcd_wq, &lcd_work);
        } else if(x_pos > SCREEN_WIDTH/2 && y_pos < SCREEN_HEIGHT/2){
            pr_info("TOP RIGHT → LED ON\n");
            led_on();
        } else {
            pr_info("BOTTOM LEFT → LED OFF\n");
            led_off();
        }
    }
}

static const struct input_device_id mouse_ids[] = {
    { .flags = INPUT_DEVICE_ID_MATCH_EVBIT,
      .evbit = { BIT_MASK(EV_KEY) | BIT_MASK(EV_REL) }, },
    {}
};

static int mouse_connect(struct input_handler *handler,
                         struct input_dev *dev,
                         const struct input_device_id *id)
{
    struct input_handle *handle;
    int error;

    handle = kzalloc(sizeof(*handle), GFP_KERNEL);
    if(!handle) return -ENOMEM;

    handle->dev = dev;
    handle->handler = handler;
    handle->name = "mouse_ili_handle";

    error = input_register_handle(handle);
    if(error) goto err_free;

    error = input_open_device(handle);
    if(error) goto err_unregister;

    pr_info("Mouse connected\n");
    return 0;

err_unregister:
    input_unregister_handle(handle);
err_free:
    kfree(handle);
    return error;
}

static void mouse_disconnect(struct input_handle *handle)
{
    input_close_device(handle);
    input_unregister_handle(handle);
    kfree(handle);
    pr_info("Mouse disconnected\n");
}

static struct input_handler mouse_handler = {
    .event      = mouse_event,
    .connect    = mouse_connect,
    .disconnect = mouse_disconnect,
    .name       = "mouse_ili_handler",
    .id_table   = mouse_ids,
};

/* ---------------- SPI probe/remove ---------------- */
static int ili9225_probe(struct spi_device *spi)
{
    struct ili9225 *lcd;

    lcd = devm_kzalloc(&spi->dev, sizeof(*lcd), GFP_KERNEL);
    if(!lcd) return -ENOMEM;

    lcd->spi = spi;
    spi_set_drvdata(spi, lcd);

    lcd->dc = devm_gpiod_get(&spi->dev, "dc", GPIOD_OUT_LOW);
    if(IS_ERR(lcd->dc)) return PTR_ERR(lcd->dc);

    lcd->reset = devm_gpiod_get(&spi->dev, "reset", GPIOD_OUT_HIGH);
    if(IS_ERR(lcd->reset)) return PTR_ERR(lcd->reset);

    /* MMIO LED */
    gpio_base = ioremap(LED_PHYS_ADDR, 0xB4);
    if(!gpio_base) return -ENOMEM;
    gpio17_set_output();
    led_off();

    /* SPI setup */
    spi->mode = SPI_MODE_0;
    spi->bits_per_word = 8;
    spi_setup(spi);

    ili9225_init(lcd);
    ili9225_fill(lcd, 0xFFFF);

    g_lcd = lcd;

    /* LCD workqueue */
    lcd_wq = create_singlethread_workqueue("lcd_wq");
    INIT_WORK(&lcd_work, lcd_work_func);

    /* Input handler */
    input_register_handler(&mouse_handler);

    /* Chardev */
    alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    cdev_init(&ili_cdev, &fops);
    cdev_add(&ili_cdev, dev_num, 1);
    ili_class = class_create(CLASS_NAME);
    device_create(ili_class, NULL, dev_num, NULL, DEVICE_NAME);

    pr_info("ILI9225 FULL DRIVER LOADED\n");
    return 0;
}

static void ili9225_remove(struct spi_device *spi)
{
    flush_workqueue(lcd_wq);
    destroy_workqueue(lcd_wq);

    input_unregister_handler(&mouse_handler);

    device_destroy(ili_class, dev_num);
    class_destroy(ili_class);
    cdev_del(&ili_cdev);
    unregister_chrdev_region(dev_num,1);

    if(gpio_base) iounmap(gpio_base);

    pr_info("ILI9225 DRIVER REMOVED\n");
}

/* Device Tree */
static const struct of_device_id ili9225_dt_ids[] = {
    { .compatible = "ilitek,ili9225" },
    {}
};
MODULE_DEVICE_TABLE(of, ili9225_dt_ids);

/* SPI driver */
static struct spi_driver ili9225_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = ili9225_dt_ids,
    },
    .probe  = ili9225_probe,
    .remove = ili9225_remove,
};

module_spi_driver(ili9225_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TEAM 2 PROJECT");
MODULE_DESCRIPTION("SPI ILI9225 driver with mouse quadrants + LED + chardev + Ctrl+C/Z support");
