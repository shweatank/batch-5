#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "oled"
#define OLED_I2C_ADDR 0x3C
#define OLED_CMD  0x00
#define OLED_DATA 0x40

static struct i2c_client *oled_client;
static int major_number;

/* Simple 5x8 font for demo (ASCII subset) */
static const u8 font5x8[][5] = {
    [' '] = {0x00,0x00,0x00,0x00,0x00},
    ['A'] = {0x7C,0x12,0x11,0x12,0x7C},
    ['B'] = {0x7F,0x49,0x49,0x49,0x36},
    ['C'] = {0x3E,0x41,0x41,0x41,0x22},
    ['H'] = {0x7F,0x08,0x08,0x08,0x7F},
    ['e'] = {0x3E,0x49,0x49,0x49,0x26},
    ['l'] = {0x00,0x41,0x7F,0x40,0x00},
    ['o'] = {0x3E,0x41,0x41,0x41,0x3E},
    // Add more as needed
};

/* Low-level I2C write */
static void oled_write_byte(u8 control, u8 data)
{
    u8 buf[2] = {control, data};
    i2c_master_send(oled_client, buf, 2);
}

static void oled_send_cmd(u8 cmd) { oled_write_byte(OLED_CMD, cmd); }
static void oled_send_data(u8 data) { oled_write_byte(OLED_DATA, data); }

/* Initialize OLED (SSD1306) */
static void oled_init_display(void)
{
    msleep(100);
    oled_send_cmd(0xAE); // Display off
    oled_send_cmd(0xD5); oled_send_cmd(0x80);
    oled_send_cmd(0xA8); oled_send_cmd(0x3F);
    oled_send_cmd(0xD3); oled_send_cmd(0x00);
    oled_send_cmd(0x40);
    oled_send_cmd(0x8D); oled_send_cmd(0x14);
    oled_send_cmd(0x20); oled_send_cmd(0x00);
    oled_send_cmd(0xA1);
    oled_send_cmd(0xC8);
    oled_send_cmd(0xDA); oled_send_cmd(0x12);
    oled_send_cmd(0x81); oled_send_cmd(0xCF);
    oled_send_cmd(0xD9); oled_send_cmd(0xF1);
    oled_send_cmd(0xDB); oled_send_cmd(0x40);
    oled_send_cmd(0xA4);
    oled_send_cmd(0xA6);
    oled_send_cmd(0xAF); // Display on
}

/* Clear OLED */
static void oled_clear_display(void)
{
    int i;
    for (i = 0; i < 128*8; i++)
        oled_send_data(0x00);
}

/* Set cursor page/column */
static void oled_set_cursor(u8 page, u8 col)
{
    oled_send_cmd(0xB0 | page);
    oled_send_cmd(0x00 | (col & 0x0F));
    oled_send_cmd(0x10 | ((col >> 4) & 0x0F));
}

/* Display a character */
static void oled_display_char(char c, u8 page, u8 col)
{
    int i;
    if (c < 32 || c > 127) c = ' ';
    oled_set_cursor(page, col);
    for (i = 0; i < 5; i++)
        oled_send_data(font5x8[(int)c][i]);
    oled_send_data(0x00);
}

/* Display string starting at page/col */
static void oled_display_string(const char *str, u8 page, u8 col)
{
    while (*str) {
        oled_display_char(*str++, page, col);
        col += 6;
        if (col + 5 >= 128) {
            col = 0;
            page++;
            if (page > 7) break;
        }
    }
}

/* Character device write */
static ssize_t oled_write(struct file *file, const char __user *buf,
                          size_t count, loff_t *ppos)
{
    char kbuf[128];
    size_t len = (count < 128) ? count : 128;

    if (copy_from_user(kbuf, buf, len))
        return -EFAULT;

    oled_clear_display();
    oled_display_string(kbuf, 0, 0);
    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = oled_write,
};

/* Probe */
static int oled_probe(struct i2c_client *client)
{
    oled_client = client;

    oled_init_display();
    oled_clear_display();

    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    pr_info("OLED device registered with major %d\n", major_number);

    return 0;
}

/* Remove */
static void oled_remove(struct i2c_client *client)
{
    unregister_chrdev(major_number, DEVICE_NAME);
    pr_info("OLED driver removed\n");
}

/* Device Tree table */
static const struct of_device_id oled_of_match[] = {
    { .compatible = "oled_ssd1306" },
    { }
};
MODULE_DEVICE_TABLE(of, oled_of_match);

/* I2C driver struct */
static struct i2c_driver oled_driver = {
    .driver = {
        .name = "oled_ssd1306",
        .of_match_table = oled_of_match,
    },
    .probe = oled_probe,
    .remove = oled_remove,
};

module_i2c_driver(oled_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LDD Project");
MODULE_DESCRIPTION("I2C OLED SSD1306 character device driver");

