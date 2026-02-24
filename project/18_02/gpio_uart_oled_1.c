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

/* ---------------- SCREEN ---------------- */
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

/* ---------------- GPIO ---------------- */
#define GPIO_BASE 0xFE200000
#define GPFSEL1 0x04
#define GPSET0  0x1C
#define GPCLR0  0x28
#define LED_GPIO 17

/* ---------------- UART ---------------- */
#define UART0_BASE 0xFE201000
#define UART_FR    0x18
#define UART_DR    0x00
#define UART_FR_TXFF (1 << 5)
#define UART_CR   0x30
#define UART_CR_UARTEN (1 << 0)
#define UART_CR_TXE   (1 << 8)

/* ---------------- OLED ---------------- */
#define OLED_ADDR 0x3C

#define DEVICE_NAME "mouse_touch"
#define MAX_NAME_LEN 64

static void __iomem *gpio_base;
static void __iomem *uart_base;
static struct i2c_client *oled_client;

/* Mouse */
static struct input_handle *mouse_handle;
static int x_pos = SCREEN_WIDTH/2;
static int y_pos = SCREEN_HEIGHT/2;

/* Workqueue */
static struct workqueue_struct *oled_wq;
static struct work_struct oled_work;

/* Stored string */
static char stored_name[MAX_NAME_LEN] = "HELLO";

/* ---------------- GPIO ---------------- */
static void gpio17_set_output(void)
{
    u32 val = ioread32(gpio_base + GPFSEL1);
    val &= ~(0x7 << 21);
    val |=  (0x1 << 21);
    iowrite32(val, gpio_base + GPFSEL1);
}

static void led_on(void)  { iowrite32(1 << LED_GPIO, gpio_base + GPSET0); }
static void led_off(void) { iowrite32(1 << LED_GPIO, gpio_base + GPCLR0); }

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

/* ---------------- OLED ---------------- */
static void oled_cmd(u8 cmd)
{
    u8 buf[2] = {0x00, cmd};
    i2c_master_send(oled_client, buf, 2);
}

static void oled_data(u8 data)
{
    u8 buf[2] = {0x40, data};
    i2c_master_send(oled_client, buf, 2);
}

static void oled_clear(void)
{
    int i;
    oled_cmd(0x21); oled_cmd(0x00); oled_cmd(0x7F);
    oled_cmd(0x22); oled_cmd(0x00); oled_cmd(0x03);
    for(i=0;i<512;i++)
        oled_data(0x00);
}

/* Very simple print (ASCII only) */
static void oled_print_string(const char *str)
{
    while(*str){
        oled_data(*str++);
    }
}

/* Workqueue function */
static void oled_display_work(struct work_struct *work)
{
    pr_info("Displaying on OLED: %s\n", stored_name);
    oled_clear();
    oled_print_string(stored_name);
}

/* ---------------- Mouse Event ---------------- */
static void mouse_event(struct input_handle *handle,
                        unsigned int type,
                        unsigned int code,
                        int value)
{
    if(type == EV_REL){
        if(code == REL_X) x_pos += value;
        if(code == REL_Y) y_pos += value;
    }

    if(type == EV_KEY && code == BTN_LEFT && value==1){

        if(x_pos>SCREEN_WIDTH/2 && y_pos<SCREEN_HEIGHT/2){
            pr_info("RIGHT TOP → LED ON\n");
            led_on();

        } else if(x_pos>SCREEN_WIDTH/2 && y_pos>SCREEN_HEIGHT/2){
            pr_info("RIGHT BOTTOM → OLED DISPLAY\n");
            queue_work(oled_wq, &oled_work);

        } else if(x_pos<SCREEN_WIDTH/2 && y_pos<SCREEN_HEIGHT/2){
            pr_info("LEFT TOP → UART SEND\n");
            uart_send_string(stored_name);
            uart_send_string("\n");

        } else {
            pr_info("LEFT BOTTOM → LED OFF\n");
            led_off();
        }
    }
}

/* ---------------- Input Handler ---------------- */
static int mouse_connect(struct input_handler *handler,
                         struct input_dev *dev,
                         const struct input_device_id *id)
{
    mouse_handle = kzalloc(sizeof(*mouse_handle), GFP_KERNEL);
    if(!mouse_handle) return -ENOMEM;

    mouse_handle->dev = dev;
    mouse_handle->handler = handler;
    mouse_handle->name = "mouse_touch";

    input_register_handle(mouse_handle);
    input_open_device(mouse_handle);

    pr_info("Mouse connected\n");
    return 0;
}

static void mouse_disconnect(struct input_handle *handle)
{
    input_close_device(handle);
    input_unregister_handle(handle);
    kfree(handle);
}

static const struct input_device_id mouse_ids[] = {
    { .flags = INPUT_DEVICE_ID_MATCH_EVBIT,
      .evbit = { BIT_MASK(EV_KEY) | BIT_MASK(EV_REL) }, },
    {}
};

static struct input_handler mouse_handler = {
    .event = mouse_event,
    .connect = mouse_connect,
    .disconnect = mouse_disconnect,
    .name = "mouse_touch_handler",
    .id_table = mouse_ids,
};

/* ---------------- Char Device ---------------- */
static ssize_t mouse_write(struct file *file,
                           const char __user *buf,
                           size_t count, loff_t *ppos)
{
    if(count >= MAX_NAME_LEN)
        count = MAX_NAME_LEN-1;

    copy_from_user(stored_name, buf, count);
    stored_name[count] = '\0';

    pr_info("Stored string: %s\n", stored_name);
    return count;
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = mouse_write,
};

/* ---------------- INIT ---------------- */
static int __init driver_init(void)
{
    struct i2c_adapter *adapter;

    pr_info("Mouse + LED + UART + OLED Driver Loading\n");

    gpio_base = ioremap(GPIO_BASE, 0xB4);
    uart_base = ioremap(UART0_BASE, 0x1000);

    gpio17_set_output();
    led_off();

    iowrite32(UART_CR_UARTEN | UART_CR_TXE, uart_base + UART_CR);

    adapter = i2c_get_adapter(1);
    oled_client = i2c_new_dummy_device(adapter, OLED_ADDR);
    i2c_put_adapter(adapter);

    oled_wq = create_singlethread_workqueue("oled_wq");
    INIT_WORK(&oled_work, oled_display_work);

    input_register_handler(&mouse_handler);

    register_chrdev(0, DEVICE_NAME, &fops);

    return 0;
}

/* ---------------- EXIT ---------------- */
static void __exit driver_exit(void)
{
    flush_workqueue(oled_wq);
    destroy_workqueue(oled_wq);
    input_unregister_handler(&mouse_handler);
    i2c_unregister_device(oled_client);
    iounmap(gpio_base);
    iounmap(uart_base);
    pr_info("Driver Removed\n");
}

module_init(driver_init);
module_exit(driver_exit);

MODULE_LICENSE("GPL");
