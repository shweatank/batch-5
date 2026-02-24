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

/* ================= Screen ================= */
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

/* ================= GPIO ================= */
#define GPIO_BASE 0xFE200000
#define GPFSEL1 0x04
#define GPSET0  0x1C
#define GPCLR0  0x28
#define LED_GPIO 17

/* ================= UART ================= */
#define UART0_BASE 0xFE201000
#define UART_FR    0x18
#define UART_DR    0x00
#define UART_FR_TXFF (1 << 5)
#define UART_CR   0x30
#define UART_CR_UARTEN (1 << 0)
#define UART_CR_TXE   (1 << 8)

/* ================= OLED ================= */
#define OLED_ADDR 0x3C
#define OLED_WIDTH 128
#define OLED_PAGES 8
#define CHAR_WIDTH 6
#define MAX_CHARS_PER_LINE (OLED_WIDTH / CHAR_WIDTH)

/* ================= Chardev ================= */
#define DEVICE_NAME "mouse_touch"
#define MAX_NAME_LEN 128

static void __iomem *gpio_base;
static void __iomem *uart_base;

static struct i2c_client *oled_client;
static struct i2c_adapter *oled_adapter;

static struct input_handle *mouse_handle;
static struct workqueue_struct *oled_wq;
static struct work_struct oled_work;

static char stored_name[MAX_NAME_LEN] = "DEFAULT";
static int major_num;

static int x_pos = SCREEN_WIDTH/2;
static int y_pos = SCREEN_HEIGHT/2;

/* ================= FONT (Basic ASCII) ================= */
static const unsigned char font5x7[128][5] = {
    [' '] = {0,0,0,0,0},
    ['A'] = {0x7E,0x11,0x11,0x11,0x7E},
    ['B'] = {0x7F,0x49,0x49,0x49,0x36},
    ['C'] = {0x3E,0x41,0x41,0x41,0x22},
    ['D'] = {0x7F,0x41,0x41,0x22,0x1C},
    ['E'] = {0x7F,0x49,0x49,0x49,0x41},
    ['H'] = {0x7F,0x08,0x08,0x08,0x7F},
    ['L'] = {0x7F,0x40,0x40,0x40,0x40},
    ['O'] = {0x3E,0x41,0x41,0x41,0x3E},
    ['0'] = {0x3E,0x51,0x49,0x45,0x3E},
    ['1'] = {0x00,0x42,0x7F,0x40,0x00},
    ['2'] = {0x42,0x61,0x51,0x49,0x46},
    ['3'] = {0x21,0x41,0x45,0x4B,0x31},
    ['4'] = {0x18,0x14,0x12,0x7F,0x10},
    ['5'] = {0x27,0x45,0x45,0x45,0x39},
    ['6'] = {0x3C,0x4A,0x49,0x49,0x30},
    ['7'] = {0x01,0x71,0x09,0x05,0x03},
    ['8'] = {0x36,0x49,0x49,0x49,0x36},
    ['9'] = {0x06,0x49,0x49,0x29,0x1E},
};

/* ================= GPIO ================= */
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

/* ================= UART ================= */
static void uart_send_char(char c)
{
    while (ioread32(uart_base + UART_FR) & UART_FR_TXFF)
        cpu_relax();
    iowrite32(c, uart_base + UART_DR);
}

static void uart_send_string(const char *str)
{
    while (*str)
        uart_send_char(*str++);
}

/* ================= OLED ================= */
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

static void oled_clear_display(void)
{
    int page, col;
    for(page=0; page<OLED_PAGES; page++){
        oled_set_cursor(page,0);
        for(col=0; col<OLED_WIDTH; col++)
            oled_write_data(0x00);
    }
}

static void oled_write_char(char c, u8 page, u8 *col)
{
    int i;
    if(c < 0 || c > 127) c = ' ';

    oled_set_cursor(page, *col);
    for(i=0;i<5;i++)
        oled_write_data(font5x7[(int)c][i]);
    oled_write_data(0x00);

    *col += CHAR_WIDTH;
}

static void oled_display_string(const char *str)
{
    u8 page=0, col=0;

    oled_clear_display();

    while(*str && page<OLED_PAGES){
        if(col >= MAX_CHARS_PER_LINE*CHAR_WIDTH){
            page++;
            col=0;
        }

        if(*str=='\n'){
            page++;
            col=0;
            str++;
            continue;
        }

        oled_write_char(*str, page, &col);
        str++;
    }
}

/* Workqueue */
static void oled_work_func(struct work_struct *work)
{
    oled_display_string(stored_name);
}

/* ================= Mouse Event ================= */
static void mouse_event(struct input_handle *handle,
                        unsigned int type,
                        unsigned int code,
                        int value)
{
    if(type==EV_REL){
        if(code==REL_X) x_pos+=value;
        if(code==REL_Y) y_pos+=value;
    }

    if(type==EV_KEY && code==BTN_LEFT && value==1){

        if(x_pos>SCREEN_WIDTH/2 && y_pos<SCREEN_HEIGHT/2){
            led_on();
        }
        else if(x_pos>SCREEN_WIDTH/2 && y_pos>SCREEN_HEIGHT/2){
            queue_work(oled_wq,&oled_work);
        }
        else if(x_pos<SCREEN_WIDTH/2 && y_pos<SCREEN_HEIGHT/2){
            uart_send_string(stored_name);
            uart_send_string("\n");
        }
        else{
            led_off();
        }
    }
}

/* ================= Char Device ================= */
static ssize_t mouse_touch_write(struct file *file,
                                 const char __user *buf,
                                 size_t count, loff_t *ppos)
{
    if(count>=MAX_NAME_LEN) count=MAX_NAME_LEN-1;

    if(copy_from_user(stored_name,buf,count))
        return -EFAULT;

    stored_name[count]='\0';
    return count;
}

static const struct file_operations fops = {
    .owner=THIS_MODULE,
    .write=mouse_touch_write,
};

/* ================= INIT ================= */
static int __init touch_driver_init(void)
{
    int ret;

    gpio_base=ioremap(GPIO_BASE,0xB4);
    gpio17_set_output();
    led_off();

    uart_base=ioremap(UART0_BASE,0x1000);
    iowrite32(UART_CR_UARTEN | UART_CR_TXE, uart_base + UART_CR);

    oled_adapter=i2c_get_adapter(1);
    oled_client=i2c_new_dummy_device(oled_adapter,OLED_ADDR);

    oled_write_cmd(0xAE);
    oled_write_cmd(0xAF);

    oled_wq=create_singlethread_workqueue("oled_wq");
    INIT_WORK(&oled_work,oled_work_func);

    ret=input_register_handler(&(struct input_handler){
        .event=mouse_event,
        .name="mouse_touch_handler",
    });

    major_num=register_chrdev(0,DEVICE_NAME,&fops);
    if(major_num<0)
        return major_num;

    pr_info("mouse_touch registered with major number %d\n",major_num);
    return 0;
}

/* ================= EXIT ================= */
static void __exit touch_driver_exit(void)
{
    unregister_chrdev(major_num,DEVICE_NAME);
    destroy_workqueue(oled_wq);
    led_off();
    iounmap(gpio_base);
    iounmap(uart_base);
    pr_info("Driver unloaded\n");
}

module_init(touch_driver_init);
module_exit(touch_driver_exit);
MODULE_LICENSE("GPL");
