#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/workqueue.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define GPIO_BASE 0xFE200000
#define GPFSEL1 0x04
#define GPSET0  0x1C
#define GPCLR0  0x28
#define LED_GPIO 17

static void __iomem *gpio_base;
static struct input_handler mouse_handler;
static struct input_handle *mouse_handle;
static int x_pos = SCREEN_WIDTH/2;
static int y_pos = SCREEN_HEIGHT/2;

/* Workqueue for LED blinking */
static struct workqueue_struct *led_wq;
static struct work_struct blink_work;
static int blink_count;

/* GPIO helper functions */
static void gpio17_set_output(void)
{
    u32 val = ioread32(gpio_base + GPFSEL1);
    val &= ~(0x7 << 21);  // clear FSEL17
    val |=  (0x1 << 21);  // set as output
    iowrite32(val, gpio_base + GPFSEL1);
}

static void led_on(void)  { iowrite32(1 << LED_GPIO, gpio_base + GPSET0); }
static void led_off(void) { iowrite32(1 << LED_GPIO, gpio_base + GPCLR0); }

/* Workqueue function for blinking LED */
static void led_blink_work(struct work_struct *work)
{
    int i;
    for(i=0;i<blink_count;i++){
        led_on();
        msleep(200);
        led_off();
        msleep(200);
    }
}

/* Mouse event callback */
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
            pr_info("RIGHT BOTTOM → LED BLINK\n");
            blink_count = 5;
            queue_work(led_wq, &blink_work);
        } else if(x_pos<SCREEN_WIDTH/2 && y_pos<SCREEN_HEIGHT/2){
            pr_info("LEFT TOP → UART message\n");
        } else {
            pr_info("LEFT BOTTOM → LED OFF\n");
            led_off();
        }
    }
}

/* Connect mouse */
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

    pr_info("Mouse connected for touch driver\n");
    return 0;
}

/* Disconnect mouse */
static void mouse_disconnect(struct input_handle *handle)
{
    input_close_device(handle);
    input_unregister_handle(handle);
    kfree(handle);
}

/* Mouse ID table */
static const struct input_device_id mouse_ids[] = {
    {
        .flags = INPUT_DEVICE_ID_MATCH_EVBIT,
        .evbit = { BIT_MASK(EV_KEY) | BIT_MASK(EV_REL) },
    },
    {}
};

MODULE_DEVICE_TABLE(input, mouse_ids);

/* Handler struct */
static struct input_handler mouse_handler = {
    .event = mouse_event,
    .connect = mouse_connect,
    .disconnect = mouse_disconnect,
    .name = "mouse_touch_handler",
    .id_table = mouse_ids,
};

/* Init module */
static int __init touch_driver_init(void)
{
    int ret;

    pr_info("Mouse Touch LED Driver Loading...\n");

    /* ioremap GPIO */
    gpio_base = ioremap(GPIO_BASE, 0xB4);
    if(!gpio_base){
        pr_err("Failed to ioremap GPIO\n");
        return -ENOMEM;
    }

    gpio17_set_output();
    led_off();

    /* Create workqueue */
    led_wq = create_singlethread_workqueue("led_blink_wq");
    if(!led_wq){
        pr_err("Failed to create workqueue\n");
        iounmap(gpio_base);
        return -ENOMEM;
    }
    INIT_WORK(&blink_work, led_blink_work);

    /* Register input handler */
    ret = input_register_handler(&mouse_handler);
    if(ret){
        destroy_workqueue(led_wq);
        iounmap(gpio_base);
        pr_err("Failed to register input handler\n");
        return ret;
    }

    return 0;
}

/* Exit module */
static void __exit touch_driver_exit(void)
{
    input_unregister_handler(&mouse_handler);
    flush_workqueue(led_wq);
    destroy_workqueue(led_wq);
    led_off();
    if(gpio_base) iounmap(gpio_base);
    pr_info("Mouse Touch LED Driver Unloaded\n");
}

module_init(touch_driver_init);
module_exit(touch_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Raspberry Pi Developer");
MODULE_DESCRIPTION("Mouse Quadrant LED Driver using ioremap GPIO17 + non-blocking blink");
