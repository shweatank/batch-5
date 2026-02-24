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
#include <linux/i2c.h>

//on oled data not recieving properly

/* ---------------- Screen ---------------- */
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

/* ---------------- GPIO ---------------- */
#define GPIO_BASE 0xFE200000
#define GPFSEL1 0x04
#define GPSET0  0x1C
#define GPCLR0  0x28
#define LED_GPIO 17

/* ---------------- UART0 ---------------- */
#define UART0_BASE 0xFE201000
#define UART_DR    0x00
#define UART_FR    0x18
#define UART_FR_TXFF (1 << 5)
#define UART_CR    0x30
#define UART_CR_UARTEN (1 << 0)
#define UART_CR_TXE   (1 << 8)

/* ---------------- OLED (I2C SSD1306) ---------------- */
#define OLED_ADDR 0x3C
#define OLED_WIDTH 128
#define OLED_PAGES 8
#define CHAR_WIDTH 6
#define MAX_CHARS_PER_LINE (OLED_WIDTH / CHAR_WIDTH)

/* ---------------- Chardev ---------------- */
#define DEVICE_NAME "mouse_touch"
#define MAX_NAME_LEN 64

static void __iomem *gpio_base;
static void __iomem *uart_base;

static struct input_handle *mouse_handle;
static int x_pos = SCREEN_WIDTH/2;
static int y_pos = SCREEN_HEIGHT/2;

static char stored_name[MAX_NAME_LEN] = "DEFAULT";
static int major_num;

static bool mouse_connected = false;

/* OLED */
static struct i2c_client *oled_client;
static struct i2c_adapter *oled_adapter;

/* Workqueue for OLED */
static struct workqueue_struct *oled_wq;
static struct work_struct oled_work;

/* ===================================================== */
/* ================= GPIO =============================== */
/* ===================================================== */

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

/* ===================================================== */
/* ================= UART =============================== */
/* ===================================================== */

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

/* ===================================================== */
/* ================= OLED =============================== */
/* ===================================================== */

static int oled_write_cmd(u8 cmd)
{
    u8 buf[2] = {0x00, cmd};
    return i2c_master_send(oled_client, buf, 2);
}

static int oled_write_data(u8 data)
{
    u8 buf[2] = {0x40, data};
    return i2c_master_send(oled_client, buf, 2);
}

static void oled_set_cursor(u8 page, u8 col)
{
    oled_write_cmd(0xB0 + page);
    oled_write_cmd(0x00 + (col & 0x0F));
    oled_write_cmd(0x10 + (col >> 4));
}

static void oled_clear(void)
{
    int page, col;

    for(page = 0; page < OLED_PAGES; page++){
        oled_set_cursor(page, 0);
        for(col = 0; col < OLED_WIDTH; col++)
            oled_write_data(0x00);
    }
}

static void oled_print_string(const char *str)
{
    int page = 0;
    int col = 0;

    oled_clear();

    while(*str && page < OLED_PAGES){

        if(col >= (MAX_CHARS_PER_LINE * CHAR_WIDTH)){
            page++;
            col = 0;
        }

        oled_set_cursor(page, col);

        /* Simple visible block character */
        oled_write_data(0x7E);
        oled_write_data(0x42);
        oled_write_data(0x42);
        oled_write_data(0x42);
        oled_write_data(0x7E);
        oled_write_data(0x00);

        col += CHAR_WIDTH;
        str++;
    }
}

/* -------- OLED Workqueue Function -------- */

static void oled_work_func(struct work_struct *work)
{
    oled_print_string(stored_name);
}

/* ===================================================== */
/* ================= MOUSE EVENT ======================== */
/* ===================================================== */

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
    }

    if(type == EV_KEY && code == BTN_LEFT && value==1){

        if(x_pos>SCREEN_WIDTH/2 && y_pos<SCREEN_HEIGHT/2){
            pr_info("RIGHT TOP → LED ON\n");
            led_on();
        }
        else if(x_pos>SCREEN_WIDTH/2 && y_pos>SCREEN_HEIGHT/2){
            pr_info("RIGHT BOTTOM → OLED DISPLAY\n");
            queue_work(oled_wq, &oled_work);
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

/* ===================================================== */
/* ================= CONNECT ============================ */
/* ===================================================== */

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

/* ===================================================== */
/* ================= CHARDEV ============================ */
/* ===================================================== */

static ssize_t mouse_touch_write(struct file *file,
                                 const char __user *buf,
                                 size_t count, loff_t *ppos)
{
    if(count >= MAX_NAME_LEN)
        count = MAX_NAME_LEN-1;

    if(copy_from_user(stored_name, buf, count))
        return -EFAULT;

    stored_name[count] = '\0';
    pr_info("Stored name: %s\n", stored_name);
    return count;
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = mouse_touch_write,
};

/* ===================================================== */
/* ================= INIT =============================== */
/* ===================================================== */

static int __init touch_driver_init(void)
{
    int ret;

    pr_info("Driver Loading...\n");

    gpio_base = ioremap(GPIO_BASE, 0xB4);
    gpio17_set_output();
    led_off();

    uart_base = ioremap(UART0_BASE, 0x1000);
    iowrite32(UART_CR_UARTEN | UART_CR_TXE, uart_base + UART_CR);

    oled_adapter = i2c_get_adapter(1);
    if(oled_adapter){
        oled_client = i2c_new_dummy_device(oled_adapter, OLED_ADDR);
        oled_write_cmd(0xAE);
        oled_write_cmd(0xAF);
    }

    oled_wq = create_singlethread_workqueue("oled_wq");
    INIT_WORK(&oled_work, oled_work_func);

    ret = input_register_handler(&mouse_handler);
    if(ret) return ret;

    major_num = register_chrdev(0, DEVICE_NAME, &fops);
    pr_info("/dev/%s Major=%d\n", DEVICE_NAME, major_num);

    return 0;
}

/* ===================================================== */
/* ================= EXIT =============================== */
/* ===================================================== */

static void __exit touch_driver_exit(void)
{
    unregister_chrdev(major_num, DEVICE_NAME);
    input_unregister_handler(&mouse_handler);

    flush_workqueue(oled_wq);
    destroy_workqueue(oled_wq);

    if(oled_client)
        i2c_unregister_device(oled_client);
    if(oled_adapter)
        i2c_put_adapter(oled_adapter);

    led_off();

    if(gpio_base) iounmap(gpio_base);
    if(uart_base) iounmap(uart_base);

    pr_info("Driver Unloaded\n");
}

module_init(touch_driver_init);
module_exit(touch_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Raspberry Pi Developer");
MODULE_DESCRIPTION("Mouse Quadrant Driver: GPIO + UART + I2C OLED + /dev using Workqueue");
