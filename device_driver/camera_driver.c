// spi_v4l2_display.c

#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/videodev2.h>
#include <linux/vmalloc.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>

#define DRIVER_NAME "spi_v4l2_display"

#define WIDTH  320
#define HEIGHT 240
#define BPP 2

struct spi_v4l2_dev {

    struct spi_device *spi;

    struct v4l2_device v4l2_dev;
    struct video_device vdev;

    u8 *framebuffer;

    struct mutex lock;
};

static int spi_send_frame(struct spi_v4l2_dev *dev)
{
    struct spi_transfer t = {
        .tx_buf = dev->framebuffer,
        .len = WIDTH * HEIGHT * BPP,
    };

    struct spi_message m;

    spi_message_init(&m);
    spi_message_add_tail(&t, &m);

    return spi_sync(dev->spi, &m);
}

static int vidioc_querycap(struct file *file, void *priv,
                           struct v4l2_capability *cap)
{
    strscpy(cap->driver, DRIVER_NAME, sizeof(cap->driver));
    strscpy(cap->card, "SPI V4L2 Display", sizeof(cap->card));

    cap->device_caps = V4L2_CAP_VIDEO_OUTPUT |
                       V4L2_CAP_STREAMING;

    cap->capabilities = cap->device_caps |
                        V4L2_CAP_DEVICE_CAPS;

    return 0;
}

static int vidioc_g_fmt(struct file *file, void *priv,
                        struct v4l2_format *f)
{
    struct v4l2_pix_format *pix = &f->fmt.pix;

    pix->width = WIDTH;
    pix->height = HEIGHT;
    pix->pixelformat = V4L2_PIX_FMT_RGB565;
    pix->field = V4L2_FIELD_NONE;
    pix->bytesperline = WIDTH * BPP;
    pix->sizeimage = WIDTH * HEIGHT * BPP;

    return 0;
}

static int vidioc_s_fmt(struct file *file, void *priv,
                        struct v4l2_format *f)
{
    return vidioc_g_fmt(file, priv, f);
}

static ssize_t v4l2_write(struct file *file,
                          const char __user *buf,
                          size_t count, loff_t *ppos)
{
    struct spi_v4l2_dev *dev = video_drvdata(file);

    if (count > WIDTH * HEIGHT * BPP)
        return -EINVAL;

    if (copy_from_user(dev->framebuffer, buf, count))
        return -EFAULT;

    spi_send_frame(dev);

    return count;
}

static const struct v4l2_ioctl_ops ioctl_ops = {

    .vidioc_querycap = vidioc_querycap,

    .vidioc_g_fmt_vid_out = vidioc_g_fmt,
    .vidioc_s_fmt_vid_out = vidioc_s_fmt,
};

static const struct v4l2_file_operations fops = {

    .owner = THIS_MODULE,
    .write = v4l2_write,
    .unlocked_ioctl = video_ioctl2,
};

static int spi_v4l2_probe(struct spi_device *spi)
{
    struct spi_v4l2_dev *dev;
    int ret;

    dev = devm_kzalloc(&spi->dev, sizeof(*dev), GFP_KERNEL);
    if (!dev)
        return -ENOMEM;

    dev->spi = spi;

    mutex_init(&dev->lock);

    dev->framebuffer = vzalloc(WIDTH * HEIGHT * BPP);
    if (!dev->framebuffer)
        return -ENOMEM;

    ret = v4l2_device_register(&spi->dev, &dev->v4l2_dev);
    if (ret)
        return ret;

    strscpy(dev->vdev.name, "spi-v4l2-display", sizeof(dev->vdev.name));

    dev->vdev.v4l2_dev = &dev->v4l2_dev;
    dev->vdev.fops = &fops;
    dev->vdev.ioctl_ops = &ioctl_ops;

    video_set_drvdata(&dev->vdev, dev);

    ret = video_register_device(&dev->vdev,
                                VFL_TYPE_VIDEO,
                                -1);
    if (ret)
        return ret;

    spi_set_drvdata(spi, dev);

    dev_info(&spi->dev, "SPI V4L2 display registered\n");

    return 0;
}

static void spi_v4l2_remove(struct spi_device *spi)
{
    struct spi_v4l2_dev *dev = spi_get_drvdata(spi);

    video_unregister_device(&dev->vdev);

    v4l2_device_unregister(&dev->v4l2_dev);

    vfree(dev->framebuffer);
}

static const struct of_device_id spi_v4l2_of_match[] = {
    { .compatible = "custom,spi-v4l2-display" },
    {}
};

MODULE_DEVICE_TABLE(of, spi_v4l2_of_match);

static struct spi_driver spi_v4l2_driver = {

    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = spi_v4l2_of_match,
    },

    .probe = spi_v4l2_probe,
    .remove = spi_v4l2_remove,
};

module_spi_driver(spi_v4l2_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Example");
MODULE_DESCRIPTION("SPI V4L2 Display Driver");
