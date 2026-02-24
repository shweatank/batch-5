#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/ioctl.h>

#define DEVICE_NAME "ili9225"
#define LCD_WIDTH   176
#define LCD_HEIGHT  220

#define ILI9225_MAGIC 'L'
#define ILI9225_FILL      _IOW(ILI9225_MAGIC, 0, unsigned short)
#define ILI9225_DRAW_PIXEL _IOW(ILI9225_MAGIC, 1, struct ili9225_pixel)

struct ili9225_pixel {
    int x;
    int y;
    unsigned short color;
};

/* Framebuffer simulation */
static uint16_t *framebuffer;

/* Char device */
static dev_t dev_num;
static struct cdev ili_cdev;
static struct class *ili_class;

/* ---------------- LOW-LEVEL ------------------- */
static void lcd_write_pixel(int x,int y,uint16_t color)
{
    if(x<0 || y<0 || x>=LCD_WIDTH || y>=LCD_HEIGHT) return;
    framebuffer[y*LCD_WIDTH + x] = color;
}

static void lcd_fill_screen(uint16_t color)
{
    int i;
    for(i=0;i<LCD_WIDTH*LCD_HEIGHT;i++)
        framebuffer[i]=color;
}

/* ---------------- FILE OPS -------------------- */
static long ili_ioctl(struct file *file,
                      unsigned int cmd,
                      unsigned long arg)
{
    unsigned short color;
    struct ili9225_pixel px;

    switch(cmd)
    {
        case ILI9225_FILL:
            if(copy_from_user(&color,(void __user*)arg,sizeof(color)))
                return -EFAULT;
            lcd_fill_screen(color);
            pr_info("ILI9225: fill 0x%04X\n",color);
            break;

        case ILI9225_DRAW_PIXEL:
            if(copy_from_user(&px,(void __user*)arg,sizeof(px)))
                return -EFAULT;
            lcd_write_pixel(px.x, px.y, px.color);
            pr_info("ILI9225: pixel x=%d y=%d color=0x%04X\n",
                    px.x, px.y, px.color);
            break;

        default: return -EINVAL;
    }
    return 0;
}

static ssize_t ili_write(struct file *file,
                         const char __user *buf,
                         size_t len,
                         loff_t *offset)
{
    unsigned short *kbuf;
    size_t count = len / sizeof(unsigned short);
    size_t i;

    kbuf = kmalloc(len,GFP_KERNEL);
    if(!kbuf) return -ENOMEM;

    if(copy_from_user(kbuf,buf,len)) { kfree(kbuf); return -EFAULT; }

    for(i=0;i<count;i++)
    {
        int x = (*offset) % LCD_WIDTH;
        int y = (*offset) / LCD_WIDTH;
        lcd_write_pixel(x,y,kbuf[i]);
        (*offset)++;
        if(*offset >= LCD_WIDTH*LCD_HEIGHT) *offset=0;
    }

    kfree(kbuf);
    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = ili_write,
    .unlocked_ioctl = ili_ioctl,
};

/* ---------------- MODULE INIT ------------------ */
static int __init ili_init(void)
{
    int ret;

    framebuffer = kzalloc(LCD_WIDTH*LCD_HEIGHT*sizeof(uint16_t),GFP_KERNEL);
    if(!framebuffer) return -ENOMEM;

    ret = alloc_chrdev_region(&dev_num,0,1,DEVICE_NAME);
    if(ret<0) { kfree(framebuffer); return ret; }

    cdev_init(&ili_cdev,&fops);
    ret = cdev_add(&ili_cdev,dev_num,1);
    if(ret<0) { unregister_chrdev_region(dev_num,1); kfree(framebuffer); return ret; }

    ili_class = class_create(DEVICE_NAME);
    if(IS_ERR(ili_class)) { cdev_del(&ili_cdev); unregister_chrdev_region(dev_num,1); kfree(framebuffer); return PTR_ERR(ili_class); }

    device_create(ili_class,NULL,dev_num,NULL,DEVICE_NAME);

    pr_info("ILI9225 driver loaded\n");
    return 0;
}

/* ---------------- MODULE EXIT ------------------ */
static void __exit ili_exit(void)
{
    device_destroy(ili_class,dev_num);
    class_destroy(ili_class);
    cdev_del(&ili_cdev);
    unregister_chrdev_region(dev_num,1);
    kfree(framebuffer);

    pr_info("ILI9225 driver removed\n");
}

module_init(ili_init);
module_exit(ili_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Generated Driver");
MODULE_DESCRIPTION("ILI9225 char driver simulation");

