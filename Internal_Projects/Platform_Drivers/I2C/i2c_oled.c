#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include"font_5x8.h"

#define WIDTH 128
#define HEIGHT 32
#define PAGES (HEIGHT / 8)

static struct i2c_client *oled_client;

static void ssd1306_write_cmd(uint8_t cmd)
{
    uint8_t buf[2] = {0x00, cmd};
    i2c_master_send(oled_client, buf, 2);
}

static void ssd1306_write_data(uint8_t data)
{
    uint8_t buf[2] = {0x40, data};
    i2c_master_send(oled_client, buf, 2);
}

static void ssd1306_set_cursor(uint8_t col, uint8_t page)
{
    ssd1306_write_cmd(0xB0 + page);
    ssd1306_write_cmd(0x00 + (col & 0x0F));
    ssd1306_write_cmd(0x10 + (col >> 4));
}

static void ssd1306_clear(void)
{
    for (uint8_t page = 0; page < PAGES; page++) {
        ssd1306_set_cursor(0, page);
        for (int col = 0; col < WIDTH; col++)
            ssd1306_write_data(0x00);
    }
}

static void ssd1306_init(void)
{
    static const uint8_t init_seq[] = {
        0xAE, 0xD5, 0x80, 0xA8, 0x1F,
        0xD3, 0x00, 0x40, 0x8D, 0x14,
        0x20, 0x00, 0xA1, 0xC8, 0xDA, 0x02,
        0x81, 0x8F, 0xD9, 0xF1, 0xDB, 0x40,
        0xA4, 0xA6, 0xAF
    };
    for (int i = 0; i < ARRAY_SIZE(init_seq); i++)
        ssd1306_write_cmd(init_seq[i]);
    msleep(100);
}

static void ssd1306_draw_char(char c)
{
    const uint8_t *bitmap;
    if( (c >127)  || (c < 32))
    {      
	    return ; 
    } 
        bitmap = font5x8[c-32];  

    for (int i = 0; i < 5; i++)
        ssd1306_write_data(bitmap[i]);
    ssd1306_write_data(0x00);  // spacing
}

static void ssd1306_draw_string(const char *str)
{
    ssd1306_set_cursor(0, 0);
    while (*str)
        ssd1306_draw_char(*str++);
}

static int ssd1306_probe(struct i2c_client *client)
{
    oled_client = client;
    dev_info(&client->dev, "SSD1306 OLED 128x32 Detected\n");

    ssd1306_init();
    ssd1306_clear();
    ssd1306_draw_string("Hema");

    return 0;
}

static void ssd1306_remove(struct i2c_client *client)
{
    dev_info(&client->dev, "SSD1306 OLED Removed\n");
    ssd1306_clear();
}

static const struct of_device_id ssd1306_dt_ids[] = {
    { .compatible = "ssd1306" },
    { }
};
MODULE_DEVICE_TABLE(of, ssd1306_dt_ids);

static struct i2c_driver ssd1306_driver = {
    .driver = {
        .name = "ssd1306_kernel_text",
        .of_match_table = ssd1306_dt_ids,
    },
    .probe = ssd1306_probe,
    .remove = ssd1306_remove,
};

module_i2c_driver(ssd1306_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hema");
MODULE_DESCRIPTION("SSD1306 Kernel Driver (128x32) - Display Text");
