// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/gpio/consumer.h>
#include <linux/delay.h>

#define LCD_WIDTH   176
#define LCD_HEIGHT  220

#define CMD_SWRESET     0x01
#define CMD_DISPLAY_OFF 0x28
#define CMD_DISPLAY_ON  0x29
#define CMD_GRAM_WRITE  0x22
#define CMD_ENTRY_MODE  0x03
#define CMD_WINDOW_X1   0x36
#define CMD_WINDOW_X2   0x37
#define CMD_WINDOW_Y1   0x38
#define CMD_WINDOW_Y2   0x39

struct ili9225 {
    struct spi_device *spi;
    struct gpio_desc *dc;
    struct gpio_desc *reset;
};

static int lcd_write(struct ili9225 *lcd, u8 data, bool is_data)
{
    gpiod_set_value(lcd->dc, is_data);
    return spi_write(lcd->spi, &data, 1);
}

static int lcd_write16(struct ili9225 *lcd, u16 data)
{
    u8 buf[2] = { data >> 8, data & 0xFF };
    gpiod_set_value(lcd->dc, 1);
    return spi_write(lcd->spi, buf, 2);
}

static void lcd_cmd(struct ili9225 *lcd, u8 cmd)
{
    lcd_write(lcd, cmd, 0);
}

static void lcd_data16(struct ili9225 *lcd, u16 data)
{
    lcd_write16(lcd, data);
}

static void lcd_reset(struct ili9225 *lcd)
{
    gpiod_set_value(lcd->reset, 0);
    msleep(20);
    gpiod_set_value(lcd->reset, 1);
    msleep(120);
}

static void ili9225_init(struct ili9225 *lcd)
{
    lcd_reset(lcd);

    lcd_cmd(lcd, CMD_SWRESET);
    msleep(50);

    lcd_cmd(lcd, CMD_DISPLAY_OFF);

    lcd_cmd(lcd, CMD_ENTRY_MODE);
    lcd_data16(lcd, 0x1030);  // RGB, horizontal increment

    lcd_cmd(lcd, CMD_DISPLAY_ON);
    msleep(50);
}

static void set_window(struct ili9225 *lcd,
                       u16 x1, u16 y1,
                       u16 x2, u16 y2)
{
    lcd_cmd(lcd, CMD_WINDOW_X1);
    lcd_data16(lcd, x1);

    lcd_cmd(lcd, CMD_WINDOW_X2);
    lcd_data16(lcd, x2);

    lcd_cmd(lcd, CMD_WINDOW_Y1);
    lcd_data16(lcd, y1);

    lcd_cmd(lcd, CMD_WINDOW_Y2);
    lcd_data16(lcd, y2);

    lcd_cmd(lcd, CMD_GRAM_WRITE);
}

static void fill_color(struct ili9225 *lcd, u16 color, int pixels)
{
    while (pixels--)
        lcd_data16(lcd, color);
}

/* 8x16 ASCII font for required characters */
static const u8 font8x16[][16] = {

/* 'J' */
{0x1E,0x0C,0x0C,0x0C,0x0C,0x6C,0x38,0x00,
 0,0,0,0,0,0,0,0},

/* 'a' */
{0x00,0x38,0x0C,0x3C,0x6C,0x6C,0x3E,0x00,
 0,0,0,0,0,0,0,0},

/* 'g' */
{0x00,0x3E,0x66,0x66,0x3E,0x06,0x3C,0x00,
 0,0,0,0,0,0,0,0},

/* 'd' */
{0x06,0x06,0x3E,0x66,0x66,0x66,0x3E,0x00,
 0,0,0,0,0,0,0,0},

/* 'i' */
{0x0C,0x00,0x1C,0x0C,0x0C,0x0C,0x1E,0x00,
 0,0,0,0,0,0,0,0},

/* 's' */
{0x00,0x3E,0x60,0x3C,0x06,0x06,0x7C,0x00,
 0,0,0,0,0,0,0,0},

/* 'h' */
{0x60,0x60,0x7C,0x66,0x66,0x66,0x66,0x00,
 0,0,0,0,0,0,0,0}
};

static void draw_char(struct ili9225 *lcd, int x, int y,
                      const u8 *bitmap, u16 fg, u16 bg)
{
    int row, col;
    set_window(lcd, x, y, x+7, y+15);

    for (row = 0; row < 16; row++) {
        for (col = 0; col < 8; col++) {
            if (bitmap[row] & (1 << (7-col)))
                lcd_data16(lcd, fg);
            else
                lcd_data16(lcd, bg);
        }
    }
}

static void draw_string(struct ili9225 *lcd, int x, int y)
{
    int i;
    const int spacing = 8;

    for (i = 0; i < 7; i++)
        draw_char(lcd, x + i*spacing, y, font8x16[i], 0xFFFF, 0x0000);
}

static int ili9225_probe(struct spi_device *spi)
{
    struct ili9225 *lcd;

    spi->mode = SPI_MODE_0;
    spi->bits_per_word = 8;
    spi_setup(spi);

    lcd = devm_kzalloc(&spi->dev, sizeof(*lcd), GFP_KERNEL);
    if (!lcd)
        return -ENOMEM;

    lcd->spi = spi;
    lcd->dc = devm_gpiod_get(&spi->dev, "dc", GPIOD_OUT_LOW);
    lcd->reset = devm_gpiod_get(&spi->dev, "reset", GPIOD_OUT_HIGH);

    ili9225_init(lcd);

    /* Clear screen */
    set_window(lcd, 0, 0, LCD_WIDTH-1, LCD_HEIGHT-1);
    fill_color(lcd, 0x0000, LCD_WIDTH * LCD_HEIGHT);

    /* Draw "Jagadish" */
    draw_string(lcd, 40, 100);

    dev_info(&spi->dev, "ILI9225: Displayed Jagadish\n");
    return 0;
}

static const struct of_device_id ili9225_of_match[] = {
    { .compatible = "ilitek,ili9225" },
    { }
};
MODULE_DEVICE_TABLE(of, ili9225_of_match);

static struct spi_driver ili9225_driver = {
    .driver = {
        .name = "ili9225_show_name",
        .of_match_table = ili9225_of_match,
    },
    .probe = ili9225_probe,
};
module_spi_driver(ili9225_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("ILI9225 display Jagadish example");
