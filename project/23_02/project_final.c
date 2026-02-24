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
#define CLASS_NAME  "ili"
#define DEVICE_NAME "ili9225_char"

#define SCREEN_WIDTH 176
#define SCREEN_HEIGHT 220
#define MAX_STRING_LEN 128

/* Example physical addresses - adjust for your board */
#define LED_PHYS_ADDR  0xFE200000  // Replace with your LED MMIO register
#define GPFSEL1 0x04
#define GPSET0 0x1c
#define GPCLR0 0x28

#define LED_GPIO 17

#define UART_PHYS_ADDR 0xFE201000  // Replace with your UART MMIO base
#define UART_FR 0x18
#define UART_DR 0x00
#define UART_FR_TXFF (1<<5)
#define UART_CR 0x30
#define UART_CR_UARTEN (1<<0)
#define UART_CR_TXE (1<<8)


/* ---------------- Globals ---------------- */
static dev_t dev_num;
static struct class *ili_class;
static struct cdev ili_cdev;
static struct ili9225 *g_lcd;

/* Cursor */
static int cursor_x = 0;
static int cursor_y = 0;

/* Workqueue for LCD */
static struct workqueue_struct *lcd_wq;
static struct work_struct lcd_work;
static char lcd_string[MAX_STRING_LEN];

/* LED / UART MMIO pointers */
static void __iomem *gpio_base;
static void __iomem *uart_base;

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
    ili9225_write_reg(lcd, 0x0001, 0x011C);
    ili9225_write_reg(lcd, 0x0002, 0x0100);
    ili9225_write_reg(lcd, 0x0003, 0x1030);
    ili9225_write_reg(lcd, 0x0008, 0x0808);
    ili9225_write_reg(lcd, 0x000C, 0x0000);
    ili9225_write_reg(lcd, 0x000F, 0x0B01);
    ili9225_write_reg(lcd, 0x0010, 0x0A00);
    ili9225_write_reg(lcd, 0x0011, 0x1038);
    msleep(50);
    ili9225_write_reg(lcd, 0x0012, 0x1121);
    ili9225_write_reg(lcd, 0x0013, 0x0063);
    ili9225_write_reg(lcd, 0x0014, 0x5A00);
    msleep(50);
    ili9225_write_reg(lcd, 0x0007, 0x1017);
    msleep(20);
}

static void ili9225_fill(struct ili9225 *lcd, u16 color)
{
    int x, y;

    ili9225_write_reg(lcd, 0x0036, 175);
    ili9225_write_reg(lcd, 0x0037, 0);
    ili9225_write_reg(lcd, 0x0038, 219);
    ili9225_write_reg(lcd, 0x0039, 0);
    ili9225_write_reg(lcd, 0x0020, 0);
    ili9225_write_reg(lcd, 0x0021, 0);

    gpiod_set_value(lcd->dc, 0);
    ili9225_write16(lcd, 0x0022);
    gpiod_set_value(lcd->dc, 1);

    for (y = 0; y < SCREEN_HEIGHT; y++)
        for (x = 0; x < SCREEN_WIDTH; x++)
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

static void drawString(int *x, int *y, const char *str, uint16_t color)
{
    int start_x = *x;
    for(int i=0; str[i]; i++)
    {
        if(str[i] == '\n') {
            *x = start_x;
            *y += 16;
            if(*y > SCREEN_HEIGHT-16) *y = 0;
            continue;
        }

        drawChar(*x, *y, str[i], color);
        *x += 16;

        if(*x > SCREEN_WIDTH - 16) {
            *x = start_x;
            *y += 16;
            if(*y > SCREEN_HEIGHT-16) *y = 0;
        }
    }
}

/* ---------------- Workqueue for LCD ---------------- */
static void lcd_work_func(struct work_struct *work)
{
    ili9225_fill(g_lcd, 0xFFFF); // white
    cursor_x = cursor_y = 0;
    drawString(&cursor_x, &cursor_y, lcd_string, 0x0000); // black
}

/* ---------------- MMIO LED ---------------- */

static void gpio17_set_output(void)
{
	u32 val = ioread32(gpio_base + GPFSEL1);
	val &= ~(0x7 << 21);
	val |= (0x1 << 21);
	iowrite32(val,gpio_base+GPFSEL1);
}

static void led_on(void)
{
	iowrite32(1<< LED_GPIO,gpio_base + GPSET0);
}
static void led_off(void)
{
	iowrite32(1<<LED_GPIO,gpio_base + GPCLR0);
}


/* ---------------- MMIO UART ---------------- */
static void uart_send_char(char c)
{
    if(!uart_base) return;

    /* Simple polling transmit */
    while(ioread32(uart_base + UART_FR) & UART_FR_TXFF)
        cpu_relax();
    iowrite32(c, uart_base+UART_DR);
}

static void uart_send_string(const char *str)
{
    while(*str){
        uart_send_char(*str++);
    }
}

/* ---------------- Chardev Write ---------------- */
static char stored_string[MAX_STRING_LEN];

static ssize_t ili_write(struct file *file,
                         const char __user *buf,
                         size_t len,
                         loff_t *off)
{
    if(len > MAX_STRING_LEN-1) len = MAX_STRING_LEN-1;
    if(copy_from_user(stored_string, buf, len)) return -EFAULT;
    stored_string[len] = '\0';
    pr_info("ili9225_char: stored string = %s\n", stored_string);
    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = ili_write,
};

/* ---------------- Mouse / Input ---------------- */
static int x_pos = SCREEN_WIDTH/2;
static int y_pos = SCREEN_HEIGHT/2;
static struct input_handle *mouse_handle;
static bool mouse_connected = false;

/* New: Right-click toggle flag */
static bool quadrant_active = false;

static void mouse_event(struct input_handle *handle,
                        unsigned int type,
                        unsigned int code,
                        int value)
{
    if(!mouse_connected) return;

    if(type == EV_REL){
        if(code == REL_X){ x_pos += value; if(x_pos<0) x_pos=0; if(x_pos>SCREEN_WIDTH) x_pos=SCREEN_WIDTH; }
        if(code == REL_Y){ y_pos += value; if(y_pos<0) y_pos=0; if(y_pos>SCREEN_HEIGHT) y_pos=SCREEN_HEIGHT; }
    }

    /* Toggle quadrant logic with right button */
    if(type == EV_KEY && code == BTN_RIGHT && value==1){
        quadrant_active = !quadrant_active;
        pr_info("Quadrant logic %s\n", quadrant_active ? "activated" : "deactivated");
        return;
    }

    /* Only execute quadrant logic if active */
    if(!quadrant_active) return;

    /* Left click still used for quadrant actions */
    if(type == EV_KEY && code == BTN_LEFT && value==1){
        if(x_pos < SCREEN_WIDTH/2 && y_pos < SCREEN_HEIGHT/2){
            pr_info("TOP LEFT → Send string via UART: %s\n", stored_string);
            uart_send_string(stored_string);
	    uart_send_string("\n");
        } else if(x_pos > SCREEN_WIDTH/2 && y_pos > SCREEN_HEIGHT/2){
            pr_info("BOTTOM RIGHT → Send string to LCD\n");
            strncpy(lcd_string, stored_string, MAX_STRING_LEN-1);
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

static int mouse_connect(struct input_handler *handler,
                         struct input_dev *dev,
                         const struct input_device_id *id)
{
    mouse_handle = kzalloc(sizeof(*mouse_handle), GFP_KERNEL);
    if(!mouse_handle) return -ENOMEM;
    mouse_handle->dev = dev;
    mouse_handle->handler = handler;
    mouse_handle->name = "mouse_ili_handle";

    if(input_register_handle(mouse_handle)){ kfree(mouse_handle); return -EINVAL; }
    if(input_open_device(mouse_handle)){ input_unregister_handle(mouse_handle); kfree(mouse_handle); return -EINVAL; }

    mouse_connected = true;
    pr_info("Mouse connected\n");
    return 0;
}

static void mouse_disconnect(struct input_handle *handle)
{
    input_close_device(handle);
    input_unregister_handle(handle);
    kfree(handle);
    mouse_connected = false;
    quadrant_active = false; // reset toggle on disconnect
}

static const struct input_device_id mouse_ids[] = {
    { .flags = INPUT_DEVICE_ID_MATCH_EVBIT, .evbit = { BIT_MASK(EV_KEY) | BIT_MASK(EV_REL) }, },
    {}
};

static struct input_handler mouse_handler = {
    .event = mouse_event,
    .connect = mouse_connect,
    .disconnect = mouse_disconnect,
    .name = "mouse_ili_handler",
    .id_table = mouse_ids,
};

/* ---------------- SPI Probe / Remove ---------------- */
static int ili9225_probe(struct spi_device *spi)
{
    struct ili9225 *lcd;
    int ret;

    lcd = devm_kzalloc(&spi->dev, sizeof(*lcd), GFP_KERNEL);
    if(!lcd) return -ENOMEM;
    lcd->spi = spi;
    spi_set_drvdata(spi, lcd);

    lcd->dc = devm_gpiod_get(&spi->dev, "dc", GPIOD_OUT_LOW);
    if(IS_ERR(lcd->dc)) return PTR_ERR(lcd->dc);

    lcd->reset = devm_gpiod_get(&spi->dev, "reset", GPIOD_OUT_HIGH);
    if(IS_ERR(lcd->reset)) return PTR_ERR(lcd->reset);

    /* MMIO LED / UART */
    gpio_base = ioremap(LED_PHYS_ADDR, 0xB4);
    if(!gpio_base)
    {
	    pr_err("Failed ioremap GPIO\n");
	    return -ENOMEM;
    }
    gpio17_set_output();
    led_off();
    uart_base = ioremap(UART_PHYS_ADDR, 0x1000);  // map 256 bytes for UART
    if(!uart_base)
    {
	    iounmap(gpio_base);
	    return -ENOMEM;
    }
    iowrite32(UART_CR_UARTEN | UART_CR_TXE, uart_base+UART_CR);


    spi->mode = SPI_MODE_0;
    spi->bits_per_word = 8;
    spi_setup(spi);

    ili9225_init(lcd);
    ili9225_fill(lcd, 0xFFFF);
    g_lcd = lcd;

    /* Chardev */
    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if(ret) return ret;
    cdev_init(&ili_cdev, &fops);
    cdev_add(&ili_cdev, dev_num, 1);
    ili_class = class_create(CLASS_NAME);
    if(IS_ERR(ili_class)){ unregister_chrdev_region(dev_num,1); return PTR_ERR(ili_class);}
    device_create(ili_class, NULL, dev_num, NULL, DEVICE_NAME);

    /* Workqueue */
    lcd_wq = create_singlethread_workqueue("lcd_wq");
    INIT_WORK(&lcd_work, lcd_work_func);

    /* Mouse handler */
    ret = input_register_handler(&mouse_handler);
    if(ret) pr_warn("Failed input_register_handler\n");

    pr_info("ILI9225 SPI + Mouse + LED + UART Driver initialized\n");
    return 0;
}

static void ili9225_remove(struct spi_device *spi)
{
    ili9225_fill(g_lcd, 0xFFFF); // white
    flush_workqueue(lcd_wq);
    destroy_workqueue(lcd_wq);

    input_unregister_handler(&mouse_handler);

    device_destroy(ili_class, dev_num);
    class_destroy(ili_class);
    cdev_del(&ili_cdev);
    unregister_chrdev_region(dev_num, 1);

    if(gpio_base) iounmap(gpio_base);
    if(uart_base) iounmap(uart_base);

    pr_info("ILI9225 SPI + Mouse + LED + UART Driver removed\n");
}

/* Device Tree */
static const struct of_device_id ili9225_dt_ids[] = {
    { .compatible = "ilitek,ili9225" },
    {}
};
MODULE_DEVICE_TABLE(of, ili9225_dt_ids);

/* SPI Driver */
static struct spi_driver ili9225_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = ili9225_dt_ids,
    },
    .probe  = ili9225_probe,
    .remove = ili9225_remove,
};

/* Module init/exit */
static int __init mouse_spi_init(void)
{
    pr_info("Registering SPI driver and mouse handler...\n");
    return spi_register_driver(&ili9225_driver);
}

static void __exit mouse_spi_exit(void)
{
    spi_unregister_driver(&ili9225_driver);
    pr_info("SPI driver unregistered\n");
}

module_init(mouse_spi_init);
module_exit(mouse_spi_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TEAM 2 PROJECT");
MODULE_DESCRIPTION("SPI ILI9225 driver integrated with mouse events, MMIO LED, MMIO UART, and workqueue LCD display with right-click toggle logic");
