#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/of.h>

#define LCD_BACKLIGHT 0x08
#define ENABLE        0x04
#define RS            0x01
#define RW            0x00  // not used, always 0

static struct i2c_client *lcd_client;

/* Write one byte to LCD via I2C */
static void lcd_write_byte(u8 data)
{
    i2c_master_send(lcd_client, &data, 1);
}

/* Toggle Enable pin with proper delay */
static void lcd_pulse_enable(u8 data)
{
    lcd_write_byte(data | ENABLE);
    msleep(5);  // pulse width ~5ms
    lcd_write_byte(data & ~ENABLE);
    msleep(5);
}

/* Write a nibble (upper 4 bits) with control flags (RS, BACKLIGHT) */
static void lcd_write_nibble(u8 nibble, u8 control)
{
    u8 data = (nibble & 0xF0) | control | LCD_BACKLIGHT;
    lcd_write_byte(data);
    lcd_pulse_enable(data);
}

/* Send command to LCD */
static void lcd_send_cmd(u8 cmd)
{
    lcd_write_nibble(cmd & 0xF0, 0);          // RS = 0 for command
    lcd_write_nibble((cmd << 4) & 0xF0, 0);  // lower nibble
}

/* Send data (character) to LCD */
static void lcd_send_data(u8 data)
{
    lcd_write_nibble(data & 0xF0, RS);          // RS = 1 for data
    lcd_write_nibble((data << 4) & 0xF0, RS);  // lower nibble
}

/* Initialize LCD with proper delays */
static void lcd_init_display(void)
{
    msleep(100);  // wait 100ms after power-on
    pr_info("lcd_disply clled\n");
    lcd_send_cmd(0x33);  // initialization sequence
    lcd_send_cmd(0x32);  // set to 4-bit mode
    lcd_send_cmd(0x28);  // 4-bit, 2-line, 5x8 dots
    lcd_send_cmd(0x0C);  // display ON, cursor OFF
    lcd_send_cmd(0x06);  // entry mode: cursor move right
    lcd_send_cmd(0x01);  // clear display
    msleep(10);          // wait after clear
    pr_info("lcd_display completed\n");
}

/* Probe function called when device is instantiated */
static int lcd_probe(struct i2c_client *client)
{
    lcd_client = client;

    pr_info("LCD LDD Driver Probed\n");

    lcd_init_display();

    /* Print "Hello LDD" on LCD */
    lcd_send_data('H');
    lcd_send_data('e');
    lcd_send_data('l');
    lcd_send_data('l');
    lcd_send_data('o');
    lcd_send_data(' ');
    lcd_send_data('L');
    lcd_send_data('D');
    lcd_send_data('D');
    pr_info("lcd_probe completed..\n");

    return 0;
}

/* Remove function */
static void lcd_remove(struct i2c_client *client)
{
    pr_info("LCD LDD Driver Removed\n");
}

/* Device Tree match table */
static const struct of_device_id lcd_of_match[] = {
    { .compatible = "lcd_ldd" },
    { }
};
MODULE_DEVICE_TABLE(of, lcd_of_match);

/* I2C Driver structure */
static struct i2c_driver lcd_driver = {
    .driver = {
        .name = "lcd_ldd",
        .of_match_table = lcd_of_match,
    },
    .probe = lcd_probe,
    .remove = lcd_remove,
};

module_i2c_driver(lcd_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LDD Project");
MODULE_DESCRIPTION("I2C LCD Driver for Raspberry Pi Kernel 6.x");

