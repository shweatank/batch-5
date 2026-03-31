#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/gpio/consumer.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>

#define DRIVER_NAME "ili9225"
#define SCREEN_WIDTH 176
#define SCREEN_HEIGHT 220
#define FRAME_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT * 2)

struct ili9225 {
    struct spi_device *spi;
    struct gpio_desc *rs;
    struct gpio_desc *reset;
    struct v4l2_device v4l2_dev;
    struct video_device *vdev;
    struct mutex lock;
};

static struct ili9225 *g_lcd;

static int ili9225_write16(struct ili9225 *lcd, u16 value)
{
    u8 buf[2];
    buf[0] = value >> 8;
    buf[1] = value & 0xFF;
    return spi_write(lcd->spi, buf, 2);
}

static int ili9225_write_reg(struct ili9225 *lcd, u16 reg, u16 data)
{
    gpiod_set_value(lcd->rs, 0);
    ili9225_write16(lcd, reg);
    gpiod_set_value(lcd->rs, 1);
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

static void ili9225_set_window(struct ili9225 *lcd)
{
    ili9225_write_reg(lcd, 0x0036, SCREEN_WIDTH - 1);
    ili9225_write_reg(lcd, 0x0037, 0);
    ili9225_write_reg(lcd, 0x0038, SCREEN_HEIGHT - 1);
    ili9225_write_reg(lcd, 0x0039, 0);
    ili9225_write_reg(lcd, 0x0020, 0);
    ili9225_write_reg(lcd, 0x0021, 0);
}
static void ili9225_fill(struct ili9225 *lcd, u16 color)
{
    int x, y;

 ili9225_set_window(lcd);

    gpiod_set_value(lcd->rs, 0);
    ili9225_write16(lcd, 0x0022);
    gpiod_set_value(lcd->rs, 1);

    for (x = 0; x < SCREEN_WIDTH; x++)
        for (y = 0; y < SCREEN_HEIGHT; y++)
            ili9225_write16(lcd, color);
}

static void ili9225_draw_image(struct ili9225 *lcd, u16 *buf)
{
    int i;
    ili9225_set_window(lcd);
    gpiod_set_value(lcd->rs, 0);
    ili9225_write16(lcd, 0x0022);
    gpiod_set_value(lcd->rs, 1);
    for (i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
        ili9225_write16(lcd, buf[i]);
}

static ssize_t ili_v4l2_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
    struct ili9225 *lcd = video_get_drvdata(video_devdata(file));
    u16 *frame;
    ssize_t ret = len;

    if (len != FRAME_SIZE)
        return -EINVAL;

    frame = kmalloc(FRAME_SIZE, GFP_KERNEL);
    if (!frame)
        return -ENOMEM;

    if (copy_from_user(frame, buf, FRAME_SIZE)) {
        kfree(frame);
        return -EFAULT;
    }

    mutex_lock(&lcd->lock);
    ili9225_draw_image(lcd, frame);
    mutex_unlock(&lcd->lock);

    kfree(frame);
    return ret;
}

static const struct v4l2_file_operations ili_v4l2_fops = {
    .owner = THIS_MODULE,
    .write = ili_v4l2_write,
    // .unlocked_ioctl, .open, .release can be added as needed
};

static int ili_querycap(struct file *file, void *priv,
                       struct v4l2_capability *cap)
{
    strscpy(cap->driver, "ili9225", sizeof(cap->driver));
    strscpy(cap->card, "ILI9225 LCD", sizeof(cap->card));
    strscpy(cap->bus_info, "spi", sizeof(cap->bus_info));

    cap->device_caps = V4L2_CAP_VIDEO_OUTPUT;
    cap->capabilities = cap->device_caps | V4L2_CAP_DEVICE_CAPS;

    return 0;
}

static const struct v4l2_ioctl_ops ili_v4l2_ioctl_ops = {
    .vidioc_querycap = ili_querycap,
};

static int ili9225_probe(struct spi_device *spi)
{
    struct ili9225 *lcd;
    int ret;

    lcd = devm_kzalloc(&spi->dev, sizeof(*lcd), GFP_KERNEL);
    if (!lcd)
        return -ENOMEM;

    lcd->spi = spi;

    lcd->rs = devm_gpiod_get(&spi->dev, "rs", GPIOD_OUT_LOW);
    lcd->reset = devm_gpiod_get(&spi->dev, "reset", GPIOD_OUT_HIGH);
    if (IS_ERR(lcd->rs) || IS_ERR(lcd->reset))
        return -ENODEV;

    spi->mode = SPI_MODE_0;
    spi->bits_per_word = 8;

    spi->max_speed_hz = 32000000;  

    ret = spi_setup(spi);
    if (ret)
        return ret;

    mutex_init(&lcd->lock);

    /* Register V4L2 device */
    ret = v4l2_device_register(&spi->dev, &lcd->v4l2_dev);
    if (ret)
        return ret;

    /* Allocate video device */
    lcd->vdev = video_device_alloc();
    if (!lcd->vdev) {
        v4l2_device_unregister(&lcd->v4l2_dev);
        return -ENOMEM;
    }

    /* Initialize video_device */
    strscpy(lcd->vdev->name, "ili9225-v4l2", sizeof(lcd->vdev->name));

    lcd->vdev->v4l2_dev   = &lcd->v4l2_dev;
    lcd->vdev->fops       = &ili_v4l2_fops;
    lcd->vdev->ioctl_ops  = &ili_v4l2_ioctl_ops;   
    lcd->vdev->release    = video_device_release;
    lcd->vdev->lock       = &lcd->lock;            
    lcd->vdev->device_caps = V4L2_CAP_VIDEO_OUTPUT; 

    video_set_drvdata(lcd->vdev, lcd);

    /* Register video device */
    ret = video_register_device(lcd->vdev, VFL_TYPE_VIDEO, -1);
    if (ret) {
        video_device_release(lcd->vdev);
        v4l2_device_unregister(&lcd->v4l2_dev);
        return ret;
    }
   g_lcd=lcd;
    /* Initialize LCD hardware */
    ili9225_init(lcd);
ili9225_fill(lcd, 0xFFFF);
    spi_set_drvdata(spi, lcd);

    pr_info( "ILI9225 V4L2 video output driver loaded - /dev/video%d\n",lcd->vdev->num);

    return 0;
}
static void ili9225_remove(struct spi_device *spi)
{
    struct ili9225 *lcd = g_lcd;
    if (!lcd)
        return;
    video_unregister_device(lcd->vdev);
    v4l2_device_unregister(&lcd->v4l2_dev);
printk(KERN_INFO "driver unloaded\n");
}

static const struct of_device_id ili9225_dt_ids[] = {
    { .compatible = "ili9225_display" },
    { }
};
MODULE_DEVICE_TABLE(of, ili9225_dt_ids);

static struct spi_driver ili9225_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = ili9225_dt_ids,
    },
    .probe = ili9225_probe,
    .remove = ili9225_remove,
};

module_spi_driver(ili9225_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Linux Kernel Developer");
MODULE_DESCRIPTION("ILI9225 SPI LCD V4L2 video output driver");
