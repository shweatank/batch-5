#include <linux/module.h>        // Core module macros
#include <linux/i2c.h>           // I2C framework
#include <linux/spi/spi.h>       // SPI framework
#include <linux/gpio.h>          // Legacy GPIO API
#include <linux/interrupt.h>     // IRQ handling
#include <linux/delay.h>         // msleep()
#include <linux/workqueue.h>     // Workqueue APIs
#include <linux/of.h>            // Device Tree support
#include "font8x8_basic.h"       // 8x8 font table for LCD text
				 

#define DS3231_SEC     0x00      // seconds register
#define DS3231_MIN     0x01      // minutes register
#define DS3231_HOUR    0x02      // hours register
#define DS3231_DAY     0x03      // day of week register
#define DS3231_DATE    0x04      // date (day of month) register
#define DS3231_MON     0x05      // month register
#define DS3231_YEAR    0x06      // year register (00–99)
#define DS3231_CONTROL 0x0E      // control register
#define DS3231_STATUS  0x0F      // status register

#define buf_size 100
#define GPIO_IR  (17 + 512)      // GPIO number for IR sensor
#define WID      176             // LCD width
#define HIG      220             // LCD height


struct my_context {
    struct i2c_client *rtc_client;       // DS3231 I2C device
    struct work_struct work;             // Workqueue job
    struct workqueue_struct *my_wq;      // Dedicated workqueue
};


struct ili9225 {
    struct spi_device *spi;      // SPI device handle
    struct gpio_desc *dc;        // Data/Command GPIO
    struct gpio_desc *reset;     // Reset GPIO
};

static struct ili9225 *g_lcd;   // Global LCD pointer
static struct my_context ctx;   // Global RTC + workqueue context
static int irq;                 // IRQ number
char kbuf[100];                 // Buffer to store RTC time string


/* Convert BCD to decimal */
static int bcd_to_dec(u8 val)
{
    return ((val >> 4) * 10) + (val & 0x0F);
}


/* Convert decimal to BCD */
static u8 dec_to_bcd(int val)
{
    return ((val / 10) << 4) | (val % 10);
}


/* Read one byte from RTC register */
static int ds3231_read_reg(struct i2c_client *client, u8 reg)
{
    return i2c_smbus_read_byte_data(client, reg);
}


/*ds3231_write_reg - Write a single byte to a DS3231 RTC register*/
static int ds3231_write_reg(struct i2c_client *client, u8 reg, u8 val)
{
    u8 buf[2];              // Buffer to hold register address + data
    int ret;                // Return value from I2C transfer

    buf[0] = reg;           // First byte: DS3231 register address
    buf[1] = val;           // Second byte: data to write into that register

    ret = i2c_master_send(client, buf, 2);   //Send 2 bytes over I2C

    /* If transfer failed, report error */
    if (ret < 0) {
        dev_err(&client->dev,"DS3231 write failed (reg=0x%02x)\n", reg);
        return -EIO;
    }

    return 0;              // Success
}


/* ds3231_set_time - Set date and time in DS3231 RTC*/
static void ds3231_set_time(struct i2c_client *client,int hour, int min, int sec,int date, int mon, int year,int day)
{
    ds3231_write_reg(client, 0x00, dec_to_bcd(sec));   // Seconds
    ds3231_write_reg(client, 0x01, dec_to_bcd(min));   // Minutes
    ds3231_write_reg(client, 0x02, dec_to_bcd(hour));  // Hours
    ds3231_write_reg(client, 0x04, dec_to_bcd(date));  // Date
    ds3231_write_reg(client, 0x05, dec_to_bcd(mon));   // Month
    ds3231_write_reg(client, 0x06, dec_to_bcd(year));  // Year (0–99)
    ds3231_write_reg(client,0x03,dec_to_bcd(day));     //day
    dev_info(&client->dev, "RTC time set\n");
}


/*ds3231_read_time - Read current date and time from DS3231 RTC */
static void ds3231_read_time(struct i2c_client *client)
{
    int sec, min, hour, day, date, mon, year;

    /* Read raw BCD values from RTC registers */
    sec  = ds3231_read_reg(client, DS3231_SEC);   // Seconds register
    min  = ds3231_read_reg(client, DS3231_MIN);   // Minutes register
    hour = ds3231_read_reg(client, DS3231_HOUR);  // Hours register
    day  = ds3231_read_reg(client, DS3231_DAY);   // Day of week
    date = ds3231_read_reg(client, DS3231_DATE);  // Date (day of month)
    mon  = ds3231_read_reg(client, DS3231_MON);   // Month
    year = ds3231_read_reg(client, DS3231_YEAR);  // Year (0–99)

    /* Check if any I2C read failed */
    if (sec < 0 || min < 0 || hour < 0) {
        dev_err(&client->dev, "RTC read failed\n");
        return;
    }

    /* Convert BCD values into decimal format */
    sec  = bcd_to_dec(sec);
    min  = bcd_to_dec(min);
    hour = bcd_to_dec(hour);
    day  = bcd_to_dec(day);
    date = bcd_to_dec(date);
    mon  = bcd_to_dec(mon & 0x1F);          //Mask century bits and convert month
    year = bcd_to_dec(year) + 2000;         //Convert year and add base (2000)

    /* Format time and date into string buffer */
    scnprintf(kbuf, buf_size,"Time: %02d:%02d:%02d Date: %02d/%02d/%d Day:%d",hour, min, sec, date, mon, year, day);

    /* Print time information to kernel log */
    pr_info("RTC Read: %s\n", kbuf);
}


/*ili9225_write16 - Send a 16-bit value to ILI9225 LCD via SPI */
static int ili9225_write16(struct ili9225 *lcd, u16 value)
{
    u8 buf[2];               //Buffer to hold two bytes for SPI transfer

    buf[0] = value >> 8;     //Extract high byte (MSB)
    buf[1] = value & 0xFF;   //Extract low byte (LSB)

    /*
     * Send both bytes over SPI in one transaction.
     * SPI sends MSB first by default.
     */
    return spi_write(lcd->spi, buf, 2);
}


/* ili9225_write_reg - Write a register and data to ILI9225 LCD */
static int ili9225_write_reg(struct ili9225 *lcd, u16 reg, u16 data)
{
    gpiod_set_value(lcd->dc, 0);        // command mode
    ili9225_write16(lcd, reg);          // send register address

    gpiod_set_value(lcd->dc, 1);        // data mode
    return ili9225_write16(lcd, data);  // send register data
}


/* ili9225_reset - Reset the ILI9225 LCD controller */
static void ili9225_reset(struct ili9225 *lcd)
{
    gpiod_set_value(lcd->reset, 1);   // keep LCD out of reset
    msleep(5);                        // short delay

    gpiod_set_value(lcd->reset, 0);   // assert reset (LCD reset)
    msleep(20);                       // wait for reset to take effect

    gpiod_set_value(lcd->reset, 1);   // release reset
    msleep(50);                       // allow LCD to stabilize
}


/* ili9225_init - Initialize the ILI9225 LCD controller */
static void ili9225_init(struct ili9225 *lcd)
{
    ili9225_reset(lcd);                        // hardware reset of LCD

    ili9225_write_reg(lcd, 0x0001, 0x011C);    // driver output control
    ili9225_write_reg(lcd, 0x0002, 0x0100);    // LCD driving waveform   
    ili9225_write_reg(lcd, 0x0003, 0x1030);    // entry mode setting
    ili9225_write_reg(lcd, 0x0008, 0x0808);    // display control
    ili9225_write_reg(lcd, 0x000C, 0x0000);    // RGB interface control
    ili9225_write_reg(lcd, 0x000F, 0x0B01);    // frame marker position

    ili9225_write_reg(lcd, 0x0010, 0x0A00);    // power control 1
    ili9225_write_reg(lcd, 0x0011, 0x1038);    // power control 2
    msleep(50);                                // power stabilization delay

    ili9225_write_reg(lcd, 0x0012, 0x1121);    // power control 3
    ili9225_write_reg(lcd, 0x0013, 0x0063);    // power control 4
    ili9225_write_reg(lcd, 0x0014, 0x5A00);    // power control 5
    msleep(50);                                // power stabilization delay

    ili9225_write_reg(lcd, 0x0007, 0x1017);    // turn ON display
    msleep(20);                                // wait for display ON
}


/* ili9225_fill - Fill entire LCD screen with a single color */
static void ili9225_fill(struct ili9225 *lcd, u16 color)
{
    int x, y;                                // loop counters for screen pixels

    ili9225_write_reg(lcd, 0x0036, 175);     // set horizontal end address
    ili9225_write_reg(lcd, 0x0037, 0);       // set horizontal start address
    ili9225_write_reg(lcd, 0x0038, 219);     // set vertical end address
    ili9225_write_reg(lcd, 0x0039, 0);       // set vertical start address
    ili9225_write_reg(lcd, 0x0020, 0);       // set X cursor position
    ili9225_write_reg(lcd, 0x0021, 0);       // set Y cursor position

    gpiod_set_value(lcd->dc, 0);             // command mode
    ili9225_write16(lcd, 0x0022);            // memory write command
    gpiod_set_value(lcd->dc, 1);             // data mode

    for (y = 0; y < HIG; y++)                // loop through rows
        for (x = 0; x < WID; x++)            // loop through columns
            ili9225_write16(lcd, color);     // write pixel color
}


/* drawPixel - Draw a single pixel on the LCD at given coordinates */
void drawChar(int x, int y, char c, uint16_t color)
{
    if (c < 32 || c > 127) return;                       // ignore non-printable characters
    const uint8_t *bitmap = font8x8[c - 32];             // get 8x8 bitmap for character

    for (int row = 0; row < 8; row++)                    // loop through font rows
    {
        for (int col = 0; col < 8; col++)                // loop through font columns
        {
            if (bitmap[row] & (1 << (7 - col)))          // check if pixel is set
            {
                drawPixel(x+col*2,  y+row*2,  color);    // draw scaled pixel
                drawPixel(x+col*2+1,y+row*2,  color);    // scale width
                drawPixel(x+col*2,  y+row*2+1,color);    // scale height
                drawPixel(x+col*2+1,y+row*2+1,color);    // scale both
            }
        }
    }
}


/* drawString - Display a string on the LCD with line wrapping */
static void drawString(int x, int y, const char *str, uint16_t color)
{
    int start_x = x;                           // store initial X position

    for (int i = 0; str[i]; i++)               // loop through characters
    {
        if (str[i] == '\n') {                  // handle new line
            x = start_x;                       // reset X to start
            y += 16;                           // move to next line
            if (y > HIG - 16) y = 0;           // wrap vertically
            continue;
        }

        drawChar(x, y, str[i], color);         // draw current character
        x += 16;                               // move cursor right

        if (x > WID - 16) {                    // wrap to next line if end reached
            x = start_x;                       // reset X
            y += 16;                           // move down
            if (y > HIG - 16) y = 0;           // wrap vertically
        }
    }
}


/* work_handler - Handle RTC read and LCD update in process context */
static void work_handler(struct work_struct *work)
{
    if (!ctx.rtc_client)                        // check if RTC device is ready
        return;

    ds3231_read_time(ctx.rtc_client);           // read current time from RTC
    pr_info("buff is %s\n", kbuf);              // debug: print time string

    ili9225_fill(g_lcd, 0xFFFF);                // clear LCD screen (white)
    drawString(10, 10, "Object\nDetected", 0x0000); // display detection message
    drawString(10, 51, kbuf, 0x0000);           // display RTC time on LCD
    enable_irq(irq);                           //enable the irq
}


/* gpio_isr - GPIO interrupt handler with debounce logic */
static irqreturn_t gpio_isr(int irq, void *dev_id)
{
     disable_irq_nosync(irq);                   //disbale the irq

     queue_work(ctx.my_wq, &ctx.work);          // schedule workqueue task

    return IRQ_HANDLED;                         // interrupt handled
}


/* rtc_probe - Probe function for DS3231 RTC I2C device */
static int rtc_probe(struct i2c_client *client)
{
    ctx.rtc_client = client;                   // store RTC device pointer

    ds3231_set_time(ctx.rtc_client,            // set initial RTC time
                    16, 12, 12,                // hour, minute, second
                    24, 02, 26,                // date, month, year
                    2);                        // day of week

    printk("RTC (I2C) probed\n");              // log probe success
    return 0;                                  // probe successful
}


static const struct of_device_id rtc_dt_ids[] = {
    { .compatible = "myvendor,myrtc" },        // match RTC device from device tree
    { }                                        // end of table
};

MODULE_DEVICE_TABLE(of, rtc_dt_ids);         // export DT match table to kernel

static struct i2c_driver rtc_driver = {
    .driver = {
        .name = "myrtc",                    // driver name shown in kernel
        .of_match_table = rtc_dt_ids,       // match device tree compatible string
    },
    .probe = rtc_probe,                     // called when RTC device is found
};


/* lcd_probe - Probe function for ILI9225 SPI LCD device */
static int lcd_probe(struct spi_device *spi)
{
    int ret;                                   // return status
    struct ili9225 *lcd;                       // LCD driver data

    lcd = devm_kzalloc(&spi->dev, sizeof(*lcd), GFP_KERNEL); // allocate LCD structure
    if (!lcd)
        return -ENOMEM;                       // allocation failed

    lcd->spi = spi;                           // store SPI device pointer
    spi_set_drvdata(spi, lcd);                // attach driver data to SPI device

    lcd->dc = devm_gpiod_get(&spi->dev, "dc", GPIOD_OUT_LOW); // get DC GPIO
    if (IS_ERR(lcd->dc))
        return PTR_ERR(lcd->dc);              // DC GPIO error

    lcd->reset = devm_gpiod_get(&spi->dev, "reset", GPIOD_OUT_HIGH); // get RESET GPIO
    if (IS_ERR(lcd->reset))
        return PTR_ERR(lcd->reset);           // RESET GPIO error

    spi->mode = SPI_MODE_0;                   // SPI mode 0
    spi->bits_per_word = 8;                   // 8-bit SPI transfers

    ret = spi_setup(spi);                     // apply SPI configuration
    if (ret)
        return ret;                           // SPI setup failed

    ili9225_init(lcd);                        // initialize LCD controller
    ili9225_fill(lcd, 0xFFFF);                // clear LCD with white color

    g_lcd = lcd;                              // store global LCD pointer

    printk("LCD (SPI) probed\n");             // log probe success
    return 0;                                 // probe successful
}


static const struct of_device_id lcd_dt_ids[] = {
    { .compatible = "myvendor,mylcd" },       // match LCD device from device tree
    { }                                       // end of match table
};

MODULE_DEVICE_TABLE(of, lcd_dt_ids);          // export DT match table to kernel

static struct spi_driver lcd_driver = {
    .driver = {
        .name = "mylcd",                      // driver name
        .of_match_table = lcd_dt_ids,         // device tree match table
    },
    .probe = lcd_probe,                       // called when LCD is detected
};


/* combined_init - Initialize RTC, LCD, GPIO interrupt and workqueue */
static int __init combined_init(void)
{
    int ret;                                            // return status

    INIT_WORK(&ctx.work, work_handler);                 // initialize workqueue job

    ctx.my_wq = create_singlethread_workqueue("worq");  // create single-thread workqueue
    if (!ctx.my_wq)
        return -ENOMEM;                                 // workqueue creation failed

    ret = i2c_add_driver(&rtc_driver);                  // register RTC I2C driver
    if (ret)
        return ret;                                     // RTC driver registration failed

    ret = spi_register_driver(&lcd_driver);             // register LCD SPI driver
    if (ret)
        i2c_del_driver(&rtc_driver);                    // cleanup RTC driver on failure

    if (!gpio_is_valid(GPIO_IR))                        // check GPIO validity
        pr_err("invalid GPIO\n");

    ret = gpio_request(GPIO_IR, "ir_gpio");             // request IR sensor GPIO
    if (ret)
        pr_info("gpio req failed\n");

    gpio_direction_input(GPIO_IR);                      // configure GPIO as input
    irq = gpio_to_irq(GPIO_IR);                         // map GPIO to IRQ number

    request_irq(irq, gpio_isr,                          // register interrupt handler
                IRQF_TRIGGER_FALLING,                   // falling edge trigger
                "ir_irq", NULL);

    printk("Combined driver loaded\n");                 // log module load
    return ret;                                         // return status
}


/* combined_exit - Cleanup RTC, LCD, GPIO interrupt and workqueue */
static void __exit combined_exit(void)
{
    ili9225_fill(g_lcd, 0xFFFF);                       // clear LCD before exit

    free_irq(irq, NULL);                               // free interrupt line
    gpio_free(GPIO_IR);                                // release IR GPIO

    destroy_workqueue(ctx.my_wq);                      // destroy workqueue

    spi_unregister_driver(&lcd_driver);                // unregister LCD SPI driver
    i2c_del_driver(&rtc_driver);                       // unregister RTC I2C driver

    printk("Combined driver unloaded\n");              // log module unload
}


module_init(combined_init);
module_exit(combined_exit);
MODULE_LICENSE("GPL");
