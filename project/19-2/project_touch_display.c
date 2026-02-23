#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/spi/spi.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define GPIO_BASE 0xFE200000
#define GPFSEL1 0x04
#define GPSET0  0x1C
#define GPCLR0  0x28
#define LED_GPIO 17

#define UART0_BASE 0xFE201000
#define UART_FR    0x18
#define UART_DR    0x00
#define UART_FR_TXFF (1 << 5)
#define UART_CR   0x30
#define UART_CR_UARTEN (1 << 0)
#define UART_CR_TXE   (1 << 8)

#define DEVICE_NAME "mouse_touch"
#define MAX_NAME_LEN 64

static void __iomem *gpio_base;
static void __iomem *uart_base;
static struct spi_device *ili_spi;

static struct input_handler mouse_handler;
static struct input_handle *mouse_handle;

static int x_pos = SCREEN_WIDTH/2;
static int y_pos = SCREEN_HEIGHT/2;

static char stored_name[MAX_NAME_LEN] = "DEFAULT";
static int major_num;

static bool mouse_connected = false;

/* ---------------- GPIO ---------------- */

static void gpio17_set_output(void)
{
    u32 val = ioread32(gpio_base + GPFSEL1);
    val &= ~(0x7 << 21);
    val |=  (0x1 << 21);
    iowrite32(val, gpio_base + GPFSEL1);
}

static void led_on(void)
{
    iowrite32(1 << LED_GPIO, gpio_base + GPSET0);
}

static void led_off(void)
{
    iowrite32(1 << LED_GPIO, gpio_base + GPCLR0);
}

/* ---------------- UART ---------------- */

static void uart_send_char(char c)
{
    while (ioread32(uart_base + UART_FR) & UART_FR_TXFF)
        cpu_relax();

    iowrite32(c, uart_base + UART_DR);
}

static void uart_send_string(const char *str)
{
    while(*str)
        uart_send_char(*str++);
}

/* ---------------- SPI DISPLAY ---------------- */

static int ili_spi_write(u16 data)
{
    u8 tx[2];
    struct spi_transfer t = {
        .tx_buf = tx,
        .len = 2,
    };
    struct spi_message m;

    if (!ili_spi)
        return -ENODEV;

    tx[0] = data >> 8;
    tx[1] = data & 0xFF;

    spi_message_init(&m);
    spi_message_add_tail(&t, &m);

    return spi_sync(ili_spi, &m);
}

static void ili9225_fill_white(void)
{
    int i;
    for(i = 0; i < 176*220; i++)
        ili_spi_write(0xFFFF);
}

static int ili_probe(struct spi_device *spi)
{
    pr_info("ILI9225 SPI Probe Success\n");
    ili_spi = spi;
    return 0;
}

static void ili_remove(struct spi_device *spi)
{
    ili_spi = NULL;
}

static const struct spi_device_id ili_ids[] = {
    { "ili9225", 0 },
    { }
};
MODULE_DEVICE_TABLE(spi, ili_ids);

static struct spi_driver ili_driver = {
    .driver = {
        .name = "ili9225",
        .owner = THIS_MODULE,
    },
    .probe = ili_probe,
    .remove = ili_remove,
    .id_table = ili_ids,
};

/* ---------------- Mouse Event ---------------- */

static void mouse_event(struct input_handle *handle,
                        unsigned int type,
                        unsigned int code,
                        int value)
{
    if(!mouse_connected)
        return;

    if(type == EV_REL){
        if(code == REL_X) x_pos += value;
        if(code == REL_Y) y_pos += value;

        if (x_pos < 0) x_pos = 0;
        if (x_pos > SCREEN_WIDTH) x_pos = SCREEN_WIDTH;
        if (y_pos < 0) y_pos = 0;
        if (y_pos > SCREEN_HEIGHT) y_pos = SCREEN_HEIGHT;
    }

    if(type == EV_KEY && code == BTN_LEFT && value==1){

        if(x_pos>SCREEN_WIDTH/2 && y_pos<SCREEN_HEIGHT/2){
            pr_info("RIGHT TOP → LED ON\n");
            led_on();
        }
        else if(x_pos>SCREEN_WIDTH/2 && y_pos>SCREEN_HEIGHT/2){
            pr_info("RIGHT BOTTOM → SPI DISPLAY\n");
            ili9225_fill_white();
        }
        else if(x_pos<SCREEN_WIDTH/2 && y_pos<SCREEN_HEIGHT/2){
            pr_info("LEFT TOP → UART SEND: %s\n", stored_name);
            uart_send_string(stored_name);
            uart_send_string("\n");
        }
        else{
            pr_info("LEFT BOTTOM → LED OFF\n");
            led_off();
        }
    }
}

/* ---------------- Mouse Connect ---------------- */

static int mouse_connect(struct input_handler *handler,
                         struct input_dev *dev,
                         const struct input_device_id *id)
{
    mouse_handle = kzalloc(sizeof(*mouse_handle), GFP_KERNEL);
    if(!mouse_handle) return -ENOMEM;

    mouse_handle->dev = dev;
    mouse_handle->handler = handler;
    mouse_handle->name = "mouse_touch_handle";

    if(input_register_handle(mouse_handle)){
        kfree(mouse_handle);
        return -EINVAL;
    }

    if(input_open_device(mouse_handle)){
        input_unregister_handle(mouse_handle);
        kfree(mouse_handle);
        return -EINVAL;
    }

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
}

static const struct input_device_id mouse_ids[] = {
    {
        .flags = INPUT_DEVICE_ID_MATCH_EVBIT,
        .evbit = { BIT_MASK(EV_KEY) | BIT_MASK(EV_REL) },
    },
    {}
};

MODULE_DEVICE_TABLE(input, mouse_ids);

static struct input_handler mouse_handler = {
    .event = mouse_event,
    .connect = mouse_connect,
    .disconnect = mouse_disconnect,
    .name = "mouse_touch_handler",
    .id_table = mouse_ids,
};

/* ---------------- Chardev ---------------- */

static ssize_t mouse_touch_write(struct file *file,
                                 const char __user *buf,
                                 size_t count, loff_t *ppos)
{
    if(count >= MAX_NAME_LEN)
        count = MAX_NAME_LEN-1;

    if(copy_from_user(stored_name, buf, count))
        return -EFAULT;

    stored_name[count] = '\0';
    return count;
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = mouse_touch_write,
};

/* ---------------- Init / Exit ---------------- */

static int __init touch_driver_init(void)
{
    int ret;

    gpio_base = ioremap(GPIO_BASE, 0xB4);
    if(!gpio_base) return -ENOMEM;

    gpio17_set_output();
    led_off();

    uart_base = ioremap(UART0_BASE, 0x1000);
    if(!uart_base){
        iounmap(gpio_base);
        return -ENOMEM;
    }

    iowrite32(UART_CR_UARTEN | UART_CR_TXE, uart_base + UART_CR);

    ret = input_register_handler(&mouse_handler);
    if(ret) return ret;

    major_num = register_chrdev(0, DEVICE_NAME, &fops);
    if(major_num < 0){
        input_unregister_handler(&mouse_handler);
        return major_num;
    }

    spi_register_driver(&ili_driver);

    pr_info("Mouse Touch + SPI Display Driver Loaded\n");
    return 0;
}

static void __exit touch_driver_exit(void)
{
    unregister_chrdev(major_num, DEVICE_NAME);
    input_unregister_handler(&mouse_handler);
    spi_unregister_driver(&ili_driver);

    led_off();

    if(gpio_base) iounmap(gpio_base);
    if(uart_base) iounmap(uart_base);

    pr_info("Driver Unloaded\n");
}

module_init(touch_driver_init);
module_exit(touch_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Raspberry Pi Developer");
MODULE_DESCRIPTION("Mouse Quadrant Driver with SPI ILI9225 Display + GPIO + UART + /dev");
