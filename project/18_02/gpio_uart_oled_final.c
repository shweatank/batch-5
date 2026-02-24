#include <linux/module.h>
#include <linux/kernel.h>
//#include <linux/device.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/i2c.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define GPIO_BASE 0xFE200000
#define GPFSEL1 0x04
#define GPSET0  0x1C
#define GPCLR0  0x28
#define LED_GPIO 17

#define UART0_BASE 0xFE201000  // PL011 UART0 base
#define UART_FR    0x18
#define UART_DR    0x00
#define UART_FR_TXFF (1 << 5)
#define UART_CR   0x30
#define UART_CR_UARTEN (1 << 0)
#define UART_CR_TXE   (1 << 8)

#define DEVICE_NAME "mouse_touch"
#define MAX_NAME_LEN 64

#define OLED_ADDR 0x3C

/* ------------------- Globals ------------------- */
static void __iomem *gpio_base;
static void __iomem *uart_base;

static struct input_handler mouse_handler;
static struct input_handle *mouse_handle;
static int x_pos = SCREEN_WIDTH/2;
static int y_pos = SCREEN_HEIGHT/2;

/* Workqueue for OLED display */
static struct workqueue_struct *oled_wq;
static struct work_struct oled_work;
static char oled_string[MAX_NAME_LEN] = "DEFAULT";

/* Chardev string */
static char stored_name[MAX_NAME_LEN] = "DEFAULT";
static int major_num;

/* Status flags */
static bool mouse_connected = false;
static bool uart_ready = false;

/* ---------------- GPIO helpers ---------------- */
static void gpio17_set_output(void)
{
    u32 val = ioread32(gpio_base + GPFSEL1);
    val &= ~(0x7 << 21);
    val |=  (0x1 << 21);
    iowrite32(val, gpio_base + GPFSEL1);
}

static void led_on(void)  { iowrite32(1 << LED_GPIO, gpio_base + GPSET0); }
static void led_off(void) { iowrite32(1 << LED_GPIO, gpio_base + GPCLR0); }

/* ---------------- UART helpers ---------------- */
static void uart_send_char(char c)
{
    if(!uart_base) return;
    while (ioread32(uart_base + UART_FR) & UART_FR_TXFF)
        cpu_relax();
    iowrite32(c, uart_base + UART_DR);
}

static void uart_send_string(const char *str)
{
    while(*str)
        uart_send_char(*str++);
}

/* ---------------- I2C OLED ---------------- */
#define OLED_ADDR 0x3C

static struct i2c_adapter *oled_adapter;
static struct i2c_client *oled_client;

/* Minimal 5x7 font table for ASCII 32-126 */
/*static const uint8_t font5x7[][5] = {
};*/

/* Very simple 5x7 font for capital letters only */
static const unsigned char font5x7[95][5] = {
{0x00,0x00,0x00,0x00,0x00}, // 32  Space
{0x00,0x00,0x5F,0x00,0x00}, // 33 !
{0x00,0x07,0x00,0x07,0x00}, // 34 "
{0x14,0x7F,0x14,0x7F,0x14}, // 35 #
{0x24,0x2A,0x7F,0x2A,0x12}, // 36 $
{0x23,0x13,0x08,0x64,0x62}, // 37 %
{0x36,0x49,0x55,0x22,0x50}, // 38 &
{0x00,0x05,0x03,0x00,0x00}, // 39 '
{0x00,0x1C,0x22,0x41,0x00}, // 40 (
{0x00,0x41,0x22,0x1C,0x00}, // 41 )
{0x14,0x08,0x3E,0x08,0x14}, // 42 *
{0x08,0x08,0x3E,0x08,0x08}, // 43 +
{0x00,0x50,0x30,0x00,0x00}, // 44 ,
{0x08,0x08,0x08,0x08,0x08}, // 45 -
{0x00,0x60,0x60,0x00,0x00}, // 46 .
{0x20,0x10,0x08,0x04,0x02}, // 47 /
{0x3E,0x51,0x49,0x45,0x3E}, // 48 0
{0x00,0x42,0x7F,0x40,0x00}, // 49 1
{0x42,0x61,0x51,0x49,0x46}, // 50 2
{0x21,0x41,0x45,0x4B,0x31}, // 51 3
{0x18,0x14,0x12,0x7F,0x10}, // 52 4
{0x27,0x45,0x45,0x45,0x39}, // 53 5
{0x3C,0x4A,0x49,0x49,0x30}, // 54 6
{0x01,0x71,0x09,0x05,0x03}, // 55 7
{0x36,0x49,0x49,0x49,0x36}, // 56 8
{0x06,0x49,0x49,0x29,0x1E}, // 57 9
{0x00,0x36,0x36,0x00,0x00}, // 58 :
{0x00,0x56,0x36,0x00,0x00}, // 59 ;
{0x08,0x14,0x22,0x41,0x00}, // 60 <
{0x14,0x14,0x14,0x14,0x14}, // 61 =
{0x00,0x41,0x22,0x14,0x08}, // 62 >
{0x02,0x01,0x51,0x09,0x06}, // 63 ?
{0x32,0x49,0x79,0x41,0x3E}, // 64 @
{0x7E,0x11,0x11,0x11,0x7E}, // 65 A
{0x7F,0x49,0x49,0x49,0x36}, // 66 B
{0x3E,0x41,0x41,0x41,0x22}, // 67 C
{0x7F,0x41,0x41,0x22,0x1C}, // 68 D
{0x7F,0x49,0x49,0x49,0x41}, // 69 E
{0x7F,0x09,0x09,0x09,0x01}, // 70 F
{0x3E,0x41,0x49,0x49,0x7A}, // 71 G
{0x7F,0x08,0x08,0x08,0x7F}, // 72 H
{0x00,0x41,0x7F,0x41,0x00}, // 73 I
{0x20,0x40,0x41,0x3F,0x01}, // 74 J
{0x7F,0x08,0x14,0x22,0x41}, // 75 K
{0x7F,0x40,0x40,0x40,0x40}, // 76 L
{0x7F,0x02,0x0C,0x02,0x7F}, // 77 M
{0x7F,0x04,0x08,0x10,0x7F}, // 78 N
{0x3E,0x41,0x41,0x41,0x3E}, // 79 O
{0x7F,0x09,0x09,0x09,0x06}, // 80 P
{0x3E,0x41,0x51,0x21,0x5E}, // 81 Q
{0x7F,0x09,0x19,0x29,0x46}, // 82 R
{0x46,0x49,0x49,0x49,0x31}, // 83 S
{0x01,0x01,0x7F,0x01,0x01}, // 84 T
{0x3F,0x40,0x40,0x40,0x3F}, // 85 U
{0x1F,0x20,0x40,0x20,0x1F}, // 86 V
{0x3F,0x40,0x38,0x40,0x3F}, // 87 W
{0x63,0x14,0x08,0x14,0x63}, // 88 X
{0x07,0x08,0x70,0x08,0x07}, // 89 Y
{0x61,0x51,0x49,0x45,0x43}, // 90 Z
{0x00,0x7F,0x41,0x41,0x00}, // 91 [
{0x02,0x04,0x08,0x10,0x20}, // 92 
{0x00,0x41,0x41,0x7F,0x00}, // 93 ]
{0x04,0x02,0x01,0x02,0x04}, // 94 ^
{0x40,0x40,0x40,0x40,0x40}, // 95 _
{0x00,0x01,0x02,0x04,0x00}, // 96 `
{0x20,0x54,0x54,0x54,0x78}, // 97 a
{0x7F,0x48,0x44,0x44,0x38}, // 98 b
{0x38,0x44,0x44,0x44,0x20}, // 99 c
{0x38,0x44,0x44,0x48,0x7F}, //100 d
{0x38,0x54,0x54,0x54,0x18}, //101 e
{0x08,0x7E,0x09,0x01,0x02}, //102 f
{0x0C,0x52,0x52,0x52,0x3E}, //103 g
{0x7F,0x08,0x04,0x04,0x78}, //104 h
{0x00,0x44,0x7D,0x40,0x00}, //105 i
{0x20,0x40,0x44,0x3D,0x00}, //106 j
{0x7F,0x10,0x28,0x44,0x00}, //107 k
{0x00,0x41,0x7F,0x40,0x00}, //108 l
{0x7C,0x04,0x18,0x04,0x78}, //109 m
{0x7C,0x08,0x04,0x04,0x78}, //110 n
{0x38,0x44,0x44,0x44,0x38}, //111 o
{0x7C,0x14,0x14,0x14,0x08}, //112 p
{0x08,0x14,0x14,0x18,0x7C}, //113 q
{0x7C,0x08,0x04,0x04,0x08}, //114 r
{0x48,0x54,0x54,0x54,0x20}, //115 s
{0x04,0x3F,0x44,0x40,0x20}, //116 t
{0x3C,0x40,0x40,0x20,0x7C}, //117 u
{0x1C,0x20,0x40,0x20,0x1C}, //118 v
{0x3C,0x40,0x30,0x40,0x3C}, //119 w
{0x44,0x28,0x10,0x28,0x44}, //120 x
{0x0C,0x50,0x50,0x50,0x3C}, //121 y
{0x44,0x64,0x54,0x4C,0x44}  //122 z
};


/* Send command to OLED */
static void oled_write_cmd(uint8_t cmd)
{
    struct i2c_msg msg;
    uint8_t buf[2] = {0x00, cmd};

    msg.addr = OLED_ADDR;
    msg.flags = 0; // write
    msg.len = 2;
    msg.buf = buf;
    i2c_transfer(oled_adapter, &msg, 1);
}

/* Send data to OLED */
static void oled_write_data(uint8_t *data, int len)
{
    struct i2c_msg msg;
    uint8_t buf[256];
    int i;

    buf[0] = 0x40; // Co=0, D/C#=1
    for(i=0;i<len;i++)
        buf[i+1] = data[i];

    msg.addr = OLED_ADDR;
    msg.flags = 0;
    msg.len = len+1;
    msg.buf = buf;
    i2c_transfer(oled_adapter, &msg, 1);
}

/* Clear OLED */
static void oled_clear(void)
{
    int page;
   // col;
    uint8_t buf[128];
    memset(buf, 0x00, 128);

    for(page=0; page<8; page++){
        oled_write_cmd(0xB0 + page);
        oled_write_cmd(0x00);
        oled_write_cmd(0x10);
        oled_write_data(buf, 128);
    }
}

/* Print string to OLED (simple 5x7 font) */
static void oled_print(const char *str)
{
    int page=0;
    int idx=0;
    uint8_t buf[6];
   // int len;

    oled_clear();
    page = 0;

    while(str[idx] && page<8){
	    int chars_per_line=21;
	    int i;

        for(i=0;i<chars_per_line && str[idx];i++,idx++){ // max chars per page
            char c = str[idx];
            if(c<32 || c>126) c=' '; // replace unsupported
            memcpy(buf, font5x7[c-32], 5);
	    buf[5]=0x00;
            oled_write_data(buf, 6);
           // oled_write_data((uint8_t[]){0x00}, 1); // space between chars
        }
        //str += 21;
        page++;
    }
}

/* Workqueue function */
static void oled_work_func(struct work_struct *work)
{
    oled_print(oled_string);
}

/* ---------------- Mouse event ---------------- */
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
        } else if(x_pos>SCREEN_WIDTH/2 && y_pos>SCREEN_HEIGHT/2){
            pr_info("RIGHT BOTTOM → OLED DISPLAY\n");
            strncpy(oled_string, stored_name, MAX_NAME_LEN-1);
            queue_work(oled_wq, &oled_work);
        } else if(x_pos<SCREEN_WIDTH/2 && y_pos<SCREEN_HEIGHT/2){
            pr_info("LEFT TOP → Sending string via UART: %s\n", stored_name);
            uart_send_string(stored_name);
            uart_send_string("\n");
        } else {
            pr_info("LEFT BOTTOM → LED OFF\n");
            led_off();
        }
    }
}

/* ---------------- Mouse connect/disconnect ---------------- */
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

/* ---------------- Input ID table ---------------- */
static const struct input_device_id mouse_ids[] = {
    {
        .flags = INPUT_DEVICE_ID_MATCH_EVBIT,
        .evbit = { BIT_MASK(EV_KEY) | BIT_MASK(EV_REL) },
    },
    {}
};
MODULE_DEVICE_TABLE(input, mouse_ids);

/* Input handler struct */
static struct input_handler mouse_handler = {
    .event = mouse_event,
    .connect = mouse_connect,
    .disconnect = mouse_disconnect,
    .name = "mouse_touch_handler",
    .id_table = mouse_ids,
};

/* ---------------- Chardev write ---------------- */
static ssize_t mouse_touch_write(struct file *file,
                                 const char __user *buf,
                                 size_t count, loff_t *ppos)
{
    if(count >= MAX_NAME_LEN)
        count = MAX_NAME_LEN-1;

    if(copy_from_user(stored_name, buf, count))
        return -EFAULT;

    stored_name[count] = '\0';
    pr_info("Chardev: stored name = %s\n", stored_name);
    return count;
}

/* File operations */
static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = mouse_touch_write,
};

/* ---------------- Module init/exit ---------------- */
static int __init touch_driver_init(void)
{
    int ret;

    pr_info("Mouse Touch LED + UART + OLED + /dev Driver Loading...\n");

    /* ioremap GPIO */
    gpio_base = ioremap(GPIO_BASE, 0xB4);
    if(!gpio_base){
        pr_err("Failed ioremap GPIO\n");
        return -ENOMEM;
    }
    gpio17_set_output();
    led_off();

    /* ioremap UART */
    uart_base = ioremap(UART0_BASE, 0x1000);
    if(!uart_base){
        iounmap(gpio_base);
        return -ENOMEM;
    }
    iowrite32(UART_CR_UARTEN | UART_CR_TXE, uart_base + UART_CR);
    uart_ready = true;

    /* I2C OLED adapter */
    oled_adapter = i2c_get_adapter(1); // bus 1
    if(!oled_adapter){
        pr_err("Failed to get I2C adapter\n");
        return -ENODEV;
    }

    oled_client=i2c_new_dummy_device(oled_adapter,OLED_ADDR);
    i2c_put_adapter(oled_adapter);
    if(!oled_client)
    {
	    pr_err("OLED cannot created\n");
	    destroy_workqueue(oled_wq);
	    iounmap(gpio_base);
	    iounmap(uart_base);
	    return -ENODEV;
    }

    /* Workqueue */
    oled_wq = create_singlethread_workqueue("oled_wq");
    if(!oled_wq){
        i2c_put_adapter(oled_adapter);
        iounmap(gpio_base);
        iounmap(uart_base);
        return -ENOMEM;
    }
    INIT_WORK(&oled_work, oled_work_func);

    /* Input handler */
    ret = input_register_handler(&mouse_handler);
    if(ret){
        destroy_workqueue(oled_wq);
        i2c_put_adapter(oled_adapter);
        iounmap(gpio_base);
        iounmap(uart_base);
        return ret;
    }

    /* Chardev */
    major_num = register_chrdev(0, DEVICE_NAME, &fops);
    if(major_num < 0){
        input_unregister_handler(&mouse_handler);
        destroy_workqueue(oled_wq);
        i2c_put_adapter(oled_adapter);
        iounmap(gpio_base);
        iounmap(uart_base);
        return major_num;
    }

    oled_clear(); // clear OLED on load

    pr_info("Driver loaded, major=%d\n", major_num);
    return 0;
}

static void __exit touch_driver_exit(void)
{
    unregister_chrdev(major_num, DEVICE_NAME);
    input_unregister_handler(&mouse_handler);

    flush_workqueue(oled_wq);
    destroy_workqueue(oled_wq);

    oled_clear(); // clear OLED on unload
    i2c_unregister_device(oled_client);

   // if(oled_adapter) i2c_put_adapter(oled_adapter);
    led_off();
    if(gpio_base) iounmap(gpio_base);
    if(uart_base) iounmap(uart_base);

    pr_info("Driver unloaded\n");
}

module_init(touch_driver_init);
module_exit(touch_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Raspberry Pi Developer");
MODULE_DESCRIPTION("Mouse Quadrant Driver with OLED I2C, UART, and /dev");

