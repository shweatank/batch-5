// ili9225_spi_full_driver.c
#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/ioctl.h>

#define DEVICE_NAME "ili9225"
#define LCD_WIDTH 176
#define LCD_HEIGHT 220

#define DC_PIN 25   // GPIO for Data/Command
#define RST_PIN 24  // GPIO for Reset

/* ---------------- Framebuffer ---------------- */
static uint16_t framebuffer[LCD_WIDTH*LCD_HEIGHT];

/* ---------------- SPI & Char Device ---------------- */
static struct spi_device *ili_spi;
static dev_t dev_num;
static struct cdev ili_cdev;
static struct class *ili_class;

/* ----------------- IOCTL Commands ---------------- */
struct ili_pixel {
    int x;
    int y;
    unsigned short color;
};

#define ILI_MAGIC 'L'
#define ILI_FILL      _IOW(ILI_MAGIC, 0, unsigned short)
#define ILI_DRAWPIXEL _IOW(ILI_MAGIC, 1, struct ili_pixel)

/* ----------------- 8x8 ASCII Font ---------------- */
/* Full ASCII 32-127 font (example: only partial, fill in your font) */
static const uint8_t font8x8[96][8] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // ' '
    {0x18,0x3C,0x3C,0x18,0x18,0x00,0x18,0x00}, // '!'
    // ... fill all 96 characters
    {0x7E,0x11,0x11,0x11,0x7E,0x00,0x00,0x00}  // 'A' as example
};

/* ----------------- SPI Low-level ---------------- */
static void lcd_spi_write(uint16_t data)
{
    uint8_t buf[2];
    buf[0] = data >> 8;
    buf[1] = data & 0xFF;
    spi_write(ili_spi, buf, 2);
}

static void lcd_write_cmd(uint16_t cmd)
{
    gpio_set_value(DC_PIN, 0);
    lcd_spi_write(cmd);
}

static void lcd_write_data(uint16_t data)
{
    gpio_set_value(DC_PIN, 1);
    lcd_spi_write(data);
}

/* ----------------- LCD Hardware ---------------- */
static void lcd_reset(void)
{
    gpio_set_value(RST_PIN, 1); msleep(5);
    gpio_set_value(RST_PIN, 0); msleep(20);
    gpio_set_value(RST_PIN, 1); msleep(50);
}

static void lcd_init_hw(void)
{
    lcd_reset();
    lcd_write_cmd(0x0001); lcd_write_data(0x011C);
    lcd_write_cmd(0x0002); lcd_write_data(0x0100);
    lcd_write_cmd(0x0003); lcd_write_data(0x1030);
    lcd_write_cmd(0x0008); lcd_write_data(0x0808);
    lcd_write_cmd(0x000C); lcd_write_data(0x0000);
    lcd_write_cmd(0x000F); lcd_write_data(0x0B01);
    lcd_write_cmd(0x0010); lcd_write_data(0x0A00);
    lcd_write_cmd(0x0011); lcd_write_data(0x1038);
    msleep(50);
    lcd_write_cmd(0x0012); lcd_write_data(0x1121);
    lcd_write_cmd(0x0013); lcd_write_data(0x0063);
    lcd_write_cmd(0x0014); lcd_write_data(0x5A00);
    msleep(50);
    lcd_write_cmd(0x0007); lcd_write_data(0x1017);
    msleep(20);
}

/* ----------------- Pixel & Char ---------------- */
static void lcd_draw_pixel(int x,int y,uint16_t color)
{
    if(x<0||y<0||x>=LCD_WIDTH||y>=LCD_HEIGHT) return;

    lcd_write_cmd(0x0020); lcd_write_data(x);
    lcd_write_cmd(0x0021); lcd_write_data(y);
    lcd_write_cmd(0x0022); lcd_write_data(color);

    framebuffer[y*LCD_WIDTH+x]=color;
}

static void lcd_fill_screen(uint16_t color)
{
    int x,y;
    for(y=0;y<LCD_HEIGHT;y++)
        for(x=0;x<LCD_WIDTH;x++)
            lcd_draw_pixel(x,y,color);
}

static void lcd_draw_char(int x,int y,char c,uint16_t color)
{
    int row,col;
    if(c<32||c>127) return;
    const uint8_t *bitmap = font8x8[c-32];
    for(row=0;row<8;row++)
        for(col=0;col<8;col++)
            if(bitmap[row] & (1<<(7-col)))
                lcd_draw_pixel(x+col,y+row,color);
}

/* ----------------- File Operations ---------------- */
static ssize_t ili_write(struct file *file,
                         const char __user *buf,
                         size_t len,
                         loff_t *offset)
{
    char kbuf[64];
    int i;
    if(len>63) len=63;
    if(copy_from_user(kbuf,buf,len)) return -EFAULT;

    for(i=0;i<len;i++)
        lcd_draw_char(i*8,0,kbuf[i],0xF800); // red text
    return len;
}

static long ili_ioctl(struct file *file,unsigned int cmd,unsigned long arg)
{
    switch(cmd)
    {
        case ILI_FILL:
        {
            unsigned short color;
            if(copy_from_user(&color,(unsigned short *)arg,sizeof(color)))
                return -EFAULT;
            lcd_fill_screen(color);
            break;
        }
        case ILI_DRAWPIXEL:
        {
            struct ili_pixel px;
            if(copy_from_user(&px,(struct ili_pixel *)arg,sizeof(px)))
                return -EFAULT;
            lcd_draw_pixel(px.x,px.y,px.color);
            break;
        }
        default:
            return -EINVAL;
    }
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = ili_write,
    .unlocked_ioctl = ili_ioctl,
};

/* ----------------- Module Init/Exit ---------------- */
static int __init ili_init(void)
{
    int ret;

    gpio_request_one(DC_PIN, GPIOF_OUT_INIT_LOW,"ili9225_dc");
    gpio_request_one(RST_PIN, GPIOF_OUT_INIT_HIGH,"ili9225_rst");

    /* SPI: bind to master 0, device 0 (adjust as needed) */
    ili_spi = spi_busnum_to_master(0) ? : NULL;
    if(!ili_spi) {
        pr_err("SPI master not found\n");
        return -ENODEV;
    }

    lcd_init_hw();

    ret = alloc_chrdev_region(&dev_num,0,1,DEVICE_NAME);
    if(ret<0) return ret;

    cdev_init(&ili_cdev,&fops);
    cdev_add(&ili_cdev,dev_num,1);

    ili_class = class_create(DEVICE_NAME);
    device_create(ili_class,NULL,dev_num,NULL,DEVICE_NAME);

    pr_info("ILI9225 full SPI driver loaded\n");
    return 0;
}

static void __exit ili_exit(void)
{
    device_destroy(ili_class,dev_num);
    class_destroy(ili_class);
    cdev_del(&ili_cdev);
    unregister_chrdev_region(dev_num,1);

    gpio_free(DC_PIN);
    gpio_free(RST_PIN);

    pr_info("ILI9225 SPI driver removed\n");
}

module_init(ili_init);
module_exit(ili_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Generated Driver");
MODULE_DESCRIPTION("Full-featured ILI9225 SPI LCD driver with ASCII support and ioctl");

