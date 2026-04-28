#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/uaccess.h>
#include <linux/ktime.h>
#include <linux/delay.h>
#include <linux/spi/spi.h>
#include "font8x8_basic.h"
#include <linux/cdev.h>


#define PROC_NAME "hcsr04_direction_led"

#define DRIVER_NAME "ili9225"
#define CLASS_NAME  "ili"
#define DEVICE_NAME "ili9225_char"
#define SCREEN_WIDTH 176
#define SCREEN_HEIGHT 220

static dev_t dev_num;
static struct class *ili_class;
static struct cdev ili_cdev;
static struct ili9225 *g_lcd;

/* Cursor position */
static int cursor_x = 0;
static int cursor_y = 0;

struct ili9225 {
    struct spi_device *spi;
    struct gpio_desc *dc;
    struct gpio_desc *reset;
};

static struct gpio_desc *trig;
static struct gpio_desc *echo;
static struct gpio_desc *led_near;   // GPIO17
static struct gpio_desc *led_far;    // GPIO27

static int irq_number;
static ktime_t echo_start, echo_end;
static unsigned long distance_cm = 0;

/* Speed variables */
static long prev_distance_cm = 0;
static ktime_t prev_time;
static long speed_cm_per_sec = 0;

static struct proc_dir_entry *proc_file;



static int ili9225_write16(struct ili9225 *lcd, u16 value)
{
    u8 buf[2];
    buf[0] = value >> 8;
    buf[1] = value & 0xFF;
    return spi_write(lcd->spi, buf, 2);
}

static int ili9225_write_reg(struct ili9225 *lcd, u16 reg, u16 data)
{
    gpiod_set_value(lcd->dc, 0);
    ili9225_write16(lcd, reg);
    gpiod_set_value(lcd->dc, 1);
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
    ili9225_write_reg(lcd, 0x0001, 0x011C);  // sets out put as top to bottom
    ili9225_write_reg(lcd, 0x0002, 0x0100);   // sets the line inversion
    ili9225_write_reg(lcd, 0x0003, 0x1030);    // GRAM entry mode
    ili9225_write_reg(lcd, 0x0008, 0x0808);   // control the frame frequency
    ili9225_write_reg(lcd, 0x000C, 0x0000);   // setting as a display control
    ili9225_write_reg(lcd, 0x000F, 0x0B01);   //power ups the lcd driver
    ili9225_write_reg(lcd, 0x0010, 0x0A00);   // internal voltages for pixels
    ili9225_write_reg(lcd, 0x0011, 0x1038); // ensures stable pixel driving voltage
    msleep(50);
    ili9225_write_reg(lcd, 0x0012, 0x1121);  //  driver output voltages
    ili9225_write_reg(lcd, 0x0013, 0x0063);  //ensures gate voltages for pixels
    ili9225_write_reg(lcd, 0x0014, 0x5A00);  // ensures uniform bright ness
    msleep(50);
    ili9225_write_reg(lcd, 0x0007, 0x1017);   // displays control on
    msleep(20);
}

// define drawing window
static void ili9225_fill(struct ili9225 *lcd, u16 color)
{
    int x, y;

    ili9225_write_reg(lcd, 0x0036, 175); //  horizontal end adress
    ili9225_write_reg(lcd, 0x0037, 0);  // horizontal syart adress
    ili9225_write_reg(lcd, 0x0038, 219); // verical end adress
    ili9225_write_reg(lcd, 0x0039, 0);   // verucal start adress
    ili9225_write_reg(lcd, 0x0020, 0);  // x_cursor position 
    ili9225_write_reg(lcd, 0x0021, 0);  // y_cursor position

    gpiod_set_value(lcd->dc, 0);    // if we set Dc=0 command mode
    ili9225_write16(lcd, 0x0022);   // GRAM data register tells next coming pixel data
    gpiod_set_value(lcd->dc, 1);     // switch data mode

    for (y = 0; y < SCREEN_HEIGHT; y++)
        for (x = 0; x < SCREEN_WIDTH; x++)
            ili9225_write16(lcd, color);      // setting the every pixel as awhite color
}

/* ---------------- Pixel / Character ---------------- */
static void drawPixel(int x, int y, uint16_t color)
{
    struct ili9225 *lcd = g_lcd;

    gpiod_set_value(lcd->dc, 0);   // command mode 
    ili9225_write16(lcd, 0x0020);  // configures where we have to write the pixels
    gpiod_set_value(lcd->dc, 1);  // data mode 
    ili9225_write16(lcd, x);      // writing the data

    gpiod_set_value(lcd->dc, 0);  
    ili9225_write16(lcd, 0x0021);
    gpiod_set_value(lcd->dc, 1);
    ili9225_write16(lcd, y);

    gpiod_set_value(lcd->dc, 0);
    ili9225_write16(lcd, 0x0022);
    gpiod_set_value(lcd->dc, 1);
    ili9225_write16(lcd, color);
}

static void drawChar(int x, int y, char c, uint16_t color)
{
    if(c < 32 || c > 127) return;                // if other than charcter then return the function otherwise continuw 
    const uint8_t *bitmap = font8x8[c - 32];

    for(int row = 0; row < 8; row++)
    {
        for(int col = 0; col < 8; col++)
        {
            if(bitmap[row] & (1 << (7 - col)))
            {
                drawPixel(x + col*2, y + row*2, color);
                drawPixel(x + col*2+1, y + row*2, color);
                drawPixel(x + col*2, y + row*2+1, color);
                drawPixel(x + col*2+1, y + row*2+1, color);
            }
        }
    }
}

static void drawString(int *x, int *y, const char *str, uint16_t color)
{
    int start_x = *x;
    for(int i=0; str[i]; i++)
    {
        if(str[i] == '\n') {
            *x = start_x;            // this part is explain if character is newline the cursor sets new line 
            *y += 16;
            if(*y > SCREEN_HEIGHT-16) *y = 0;
            continue;
        }

        drawChar(*x, *y, str[i], color);       // if not print the character on lcd display
        *x += 16;

        if(*x > SCREEN_WIDTH - 16) {
            *x = start_x;
            *y += 16;
            if(*y > SCREEN_HEIGHT-16) *y = 0;
        }
    }
}

/* ---------------- Interrupt Handler ---------------- */
static irqreturn_t echo_irq_handler(int irq, void *dev_id)
{
    if (gpiod_get_value(echo))
        echo_start = ktime_get();
    else {
        echo_end = ktime_get();
        s64 duration_us = ktime_to_us(ktime_sub(echo_end, echo_start));
        if (duration_us > 0)
            distance_cm = duration_us / 58;   // Distance in cm
    }

    return IRQ_HANDLED;
}

/* ---------------- PROC READ FUNCTION ---------------- */
static ssize_t proc_read(struct file *file, char __user *buf,
                         size_t count, loff_t *pos)
{
    char output[128];
    int len, i;
    ktime_t current_time;
    s64 time_diff_ms;
    long distance_diff;
    long display_speed = 0;
    unsigned int blink_delay = 150;

    if (*pos > 0)
        return 0;

    /* Trigger ultrasonic sensor */
    gpiod_set_value(trig, 0);
    udelay(2);
    gpiod_set_value(trig, 1);
    udelay(10);
    gpiod_set_value(trig, 0);

    msleep(60);

    /* ---- Speed Calculation ---- */
    current_time = ktime_get();
    time_diff_ms = ktime_to_ms(ktime_sub(current_time, prev_time));

    if (time_diff_ms > 0) {
        distance_diff = distance_cm - prev_distance_cm;
        speed_cm_per_sec = (distance_diff * 1000) / time_diff_ms;
    }

    prev_distance_cm = distance_cm;
    prev_time = current_time;

    /* Absolute value for display */
    if (speed_cm_per_sec < 0)
        display_speed = -speed_cm_per_sec;
    else
        display_speed = speed_cm_per_sec;

    /* Turn off both LEDs */
    gpiod_set_value(led_near, 0);
    gpiod_set_value(led_far, 0);

    /* ---- Direction Based LED Blink ---- */

    if (speed_cm_per_sec < 0) {
        /* Object moving closer */
        for (i = 0; i < 5; i++) {
            gpiod_set_value(led_near, 1);
            msleep(blink_delay);
            gpiod_set_value(led_near, 0);
            msleep(blink_delay);
        }
    }
    else if (speed_cm_per_sec > 0) {
        /* Object moving away */
        for (i = 0; i < 5; i++) {
            gpiod_set_value(led_far, 1);
            msleep(blink_delay);
            gpiod_set_value(led_far, 0);
            msleep(blink_delay);
        }
    }

    /* Output to user */
    len = sprintf(output,
                  "Distance: \n\n%lu cm\n\nSpeed: \n\n%ld cm/s\n",
                  distance_cm,
                  display_speed);

    ili9225_fill(g_lcd, 0xFFFF);  // white background
    cursor_x = 0;
    cursor_y = 20;

    /* Draw the new string */
    drawString(&cursor_x, &cursor_y, output, 0x0000); // black text


    if (copy_to_user(buf, output, len))
        return -EFAULT;

    *pos = len;
    return len;
}
static int ili9225_probe(struct spi_device *spi)
{
    struct ili9225 *lcd;

    lcd = devm_kzalloc(&spi->dev, sizeof(*lcd), GFP_KERNEL);
    if (!lcd) return -ENOMEM;    //kernel allcates the memory using kzalloc api

    lcd->spi = spi;
    spi_set_drvdata(spi, lcd);

    lcd->dc = devm_gpiod_get(&spi->dev, "dc", GPIOD_OUT_LOW);
    if (IS_ERR(lcd->dc)) return PTR_ERR(lcd->dc);

    lcd->reset = devm_gpiod_get(&spi->dev, "reset", GPIOD_OUT_HIGH);
    if (IS_ERR(lcd->reset)) return PTR_ERR(lcd->reset);

    spi->mode = SPI_MODE_0;  // CPOL =0,CPHA = 0
    spi->bits_per_word = 8;  // we have to send 8 bits at a time
    spi_setup(spi);

    ili9225_init(lcd);           // lcd init function
    ili9225_fill(lcd, 0xFFFF);  // setting height and width of lcd display

    g_lcd = lcd;

    pr_info( "ILI9225 initialized\n");


  return 0;
}
static void ili9225_remove(struct spi_device *spi)
{
    device_destroy(ili_class, dev_num);
    class_destroy(ili_class);
    cdev_del(&ili_cdev);
    unregister_chrdev_region(dev_num, 1);

    pr_info("ILI9225 removed\n");
}


/* ---------------- Device Tree ---------------- */
static const struct of_device_id ili9225_dt_ids[] = {
    { .compatible = "ilitek,ili9225" },
    { }
};  // for matching the device tree configuration
MODULE_DEVICE_TABLE(of, ili9225_dt_ids);

/* ---------------- SPI Driver ---------------- */
static struct spi_driver ili9225_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = ili9225_dt_ids,
    },
    .probe  = ili9225_probe,
    .remove = ili9225_remove,
};

//module_spi_driver(ili9225_driver);    //load the driver when inserted using insmod

static const struct proc_ops proc_fops = {
    .proc_read = proc_read,
};

/* ---------------- Device Tree Match ---------------- */
static const struct of_device_id hcsr04_of_match[] = {
    { .compatible = "mycompany,hcsr04", },
    {},
};
MODULE_DEVICE_TABLE(of, hcsr04_of_match);

/* ---------------- Probe ---------------- */
static int hcsr04_probe(struct platform_device *pdev)
{
    int ret;

    trig = devm_gpiod_get(&pdev->dev, "trig", GPIOD_OUT_LOW);
    echo = devm_gpiod_get(&pdev->dev, "echo", GPIOD_IN);
    led_near = devm_gpiod_get(&pdev->dev, "led-near", GPIOD_OUT_LOW);
    led_far  = devm_gpiod_get(&pdev->dev, "led-far", GPIOD_OUT_LOW);

    if (IS_ERR(trig) || IS_ERR(echo) ||
        IS_ERR(led_near) || IS_ERR(led_far)) {
        dev_err(&pdev->dev, "Failed to get GPIOs\n");
        return -ENODEV;
    }

    irq_number = gpiod_to_irq(echo);
    if (irq_number < 0)
        return irq_number;

    ret = request_irq(irq_number,
                      echo_irq_handler,
                      IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                      "hcsr04_irq",
                      NULL);
    if (ret)
        return ret;

    proc_file = proc_create(PROC_NAME, 0666, NULL, &proc_fops);
    if (!proc_file) {
        free_irq(irq_number, NULL);
        return -ENOMEM;
    }

    prev_time = ktime_get();

    dev_info(&pdev->dev, "HC-SR04 Direction Detection Driver Loaded\n");
    return 0;
}

/* ---------------- Remove ---------------- */
static void hcsr04_remove(struct platform_device *pdev)
{
    proc_remove(proc_file);
    free_irq(irq_number, NULL);

    gpiod_set_value(led_near, 0);
    gpiod_set_value(led_far, 0);

    //dev_info(&pdev->dev, "HC-SR04 Driver Unloaded\n");
}

static struct platform_driver hcsr04_driver = {
    .probe  = hcsr04_probe,
    .remove = hcsr04_remove,
    .driver = {
        .name = "hcsr04_direction_led",
        .of_match_table = hcsr04_of_match,
    },
};

//module_platform_driver(hcsr04_driver);

static int __init my_driver_init(void)
{
    int ret;

    ret = spi_register_driver(&ili9225_driver);
    if (ret)
        return ret;

    ret = platform_driver_register(&hcsr04_driver);
    if (ret) {
        spi_unregister_driver(&ili9225_driver);
        return ret;
    }

    return 0;
}

static void __exit my_driver_exit(void)
{
    platform_driver_unregister(&hcsr04_driver);
    spi_unregister_driver(&ili9225_driver);
}

module_init(my_driver_init);
module_exit(my_driver_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("team 4");
MODULE_DESCRIPTION("HC-SR04 Direction Detection with Two LEDs (Positive Speed Output)");
