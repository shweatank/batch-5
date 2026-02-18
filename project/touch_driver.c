#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/fs.h>
#include <linux/input.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/uaccess.h>

#define LED_GPIO 17
#define DEVICE_PATH "/dev/input/event3"   // CHANGE if needed
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

static struct task_struct *mouse_thread;
static struct file *file;

static int x_pos = SCREEN_WIDTH / 2;
static int y_pos = SCREEN_HEIGHT / 2;

static void led_blink(void)
{
    int i;
    for (i = 0; i < 5; i++) {
        gpio_set_value(LED_GPIO, 1);
        msleep(200);
        gpio_set_value(LED_GPIO, 0);
        msleep(200);
    }
}

static int mouse_fn(void *data)
{
    struct input_event ev;
    mm_segment_t old_fs;

    old_fs = get_fs();
    set_fs(KERNEL_DS);

    file = filp_open(DEVICE_PATH, O_RDONLY, 0);
    if (IS_ERR(file)) {
        pr_err("Cannot open input device\n");
        set_fs(old_fs);
        return -1;
    }

    pr_info("Mouse device opened\n");

    while (!kthread_should_stop()) {

        kernel_read(file, &ev, sizeof(ev), &file->f_pos);

        if (ev.type == EV_REL) {
            if (ev.code == REL_X)
                x_pos += ev.value;
            if (ev.code == REL_Y)
                y_pos += ev.value;
        }

        if (ev.type == EV_KEY && ev.code == BTN_LEFT && ev.value == 1) {

            pr_info("Click at X:%d Y:%d\n", x_pos, y_pos);

            if (x_pos > SCREEN_WIDTH/2 && y_pos < SCREEN_HEIGHT/2) {
                gpio_set_value(LED_GPIO, 1);
                pr_info("LED ON\n");
            }
            else if (x_pos > SCREEN_WIDTH/2 && y_pos > SCREEN_HEIGHT/2) {
                led_blink();
                pr_info("LED BLINK\n");
            }
            else if (x_pos < SCREEN_WIDTH/2 && y_pos < SCREEN_HEIGHT/2) {
                pr_info("UART MESSAGE\n");
            }
            else {
                gpio_set_value(LED_GPIO, 0);
                pr_info("LED OFF\n");
            }
        }

        msleep(10);
    }

    filp_close(file, NULL);
    set_fs(old_fs);
    return 0;
}

static int __init mouse_init(void)
{
    int ret;

    pr_info("Mouse Touch Driver Loaded\n");

    ret = gpio_request(LED_GPIO, "led_gpio");
    if (ret)
        return ret;

    gpio_direction_output(LED_GPIO, 0);

    mouse_thread = kthread_run(mouse_fn, NULL, "mouse_thread");
    if (IS_ERR(mouse_thread)) {
        gpio_free(LED_GPIO);
        return PTR_ERR(mouse_thread);
    }

    return 0;
}

static void __exit mouse_exit(void)
{
    if (mouse_thread)
        kthread_stop(mouse_thread);

    gpio_set_value(LED_GPIO, 0);
    gpio_free(LED_GPIO);

    pr_info("Mouse Touch Driver Unloaded\n");
}

module_init(mouse_init);
module_exit(mouse_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("RPI Developer");
MODULE_DESCRIPTION("Mouse Based LED Driver");
