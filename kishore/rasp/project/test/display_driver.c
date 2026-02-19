#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/init.h>

#define SCK 11   // BCM11
#define MOSI 10  // BCM10
#define DC 25    // BCM25
#define RST 24   // BCM24
#define CS 8     // BCM8 (CE0)

// Helper: set SPI pins
static void spi_write_bit(int bit)
{
    gpio_set_value(MOSI, bit);
    udelay(1);
    gpio_set_value(SCK, 1);
    udelay(1);
    gpio_set_value(SCK, 0);
    udelay(1);
}

// Write 16-bit command/data
static void spi_write16(u16 val)
{
    int i;
    for (i = 15; i >= 0; i--)
        spi_write_bit((val >> i) & 0x1);
}

// Send command to ILI9225
static void write_command(u16 cmd)
{
    gpio_set_value(DC, 0); // command
    spi_write16(cmd);
}

// Send data to ILI9225
static void write_data(u16 data)
{
    gpio_set_value(DC, 1); // data
    spi_write16(data);
}

// Reset LCD
static void lcd_reset(void)
{
    gpio_set_value(RST, 1); mdelay(5);
    gpio_set_value(RST, 0); mdelay(20);
    gpio_set_value(RST, 1); mdelay(50);
}

// Minimal init sequence
static void lcd_init(void)
{
    lcd_reset();

    write_command(0x0001); write_data(0x011C);
    write_command(0x0002); write_data(0x0100);
    write_command(0x0003); write_data(0x1030);
    write_command(0x0008); write_data(0x0808);
    write_command(0x000C); write_data(0x0000);
    write_command(0x000F); write_data(0x0B01);

    write_command(0x0010); write_data(0x0A00);
    write_command(0x0011); write_data(0x1038);
    mdelay(50);
    write_command(0x0012); write_data(0x1121);
    write_command(0x0013); write_data(0x0063);
    write_command(0x0014); write_data(0x5A00);
    mdelay(50);

    write_command(0x0007); write_data(0x1017);
    mdelay(20);
}

// Fill screen with a single color
static void lcd_fill(u16 color)
{
    int x, y;
    write_command(0x0036); write_data(175);
    write_command(0x0037); write_data(0);
    write_command(0x0038); write_data(219);
    write_command(0x0039); write_data(0);
    write_command(0x0020); write_data(0);
    write_command(0x0021); write_data(0);

    write_command(0x0022); // RAM write
    gpio_set_value(DC, 1);

    for (y = 0; y < 220; y++)
        for (x = 0; x < 176; x++)
            spi_write16(color);
}

// Draw minimal "HI" pattern
static void draw_HI(void)
{
    int x, y;
    u16 color = 0x0000; // black

    // Draw H
    for (y = 20; y < 60; y++)
    {
        spi_write16(color); // left vertical
        spi_write16(color); // right vertical
    }

    // Draw I
    for (y = 20; y < 60; y++)
    {
        spi_write16(color);
    }
}

// Module init
static int __init display_driver_init(void)
{
    int ret;

    // Request GPIOs
    ret = gpio_request_one(SCK, GPIOF_OUT_INIT_LOW, "SCK"); if(ret) return ret;
    ret = gpio_request_one(MOSI, GPIOF_OUT_INIT_LOW, "MOSI"); if(ret) return ret;
    ret = gpio_request_one(DC, GPIOF_OUT_INIT_LOW, "DC"); if(ret) return ret;
    ret = gpio_request_one(RST, GPIOF_OUT_INIT_HIGH, "RST"); if(ret) return ret;
    ret = gpio_request_one(CS, GPIOF_OUT_INIT_HIGH, "CS"); if(ret) return ret;

    pr_info("ILI9225 driver loaded\n");

    lcd_init();
    lcd_fill(0xFFFF); // white
    draw_HI();

    return 0;
}

// Module exit
static void __exit display_driver_exit(void)
{
    gpio_set_value(SCK, 0);
    gpio_set_value(MOSI, 0);
    gpio_set_value(DC, 0);
    gpio_set_value(RST, 0);
    gpio_set_value(CS, 1);

    gpio_free(SCK);
    gpio_free(MOSI);
    gpio_free(DC);
    gpio_free(RST);
    gpio_free(CS);

    pr_info("ILI9225 driver unloaded\n");
}

module_init(display_driver_init);
module_exit(display_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("team4");
MODULE_DESCRIPTION("Minimal ILI9225 bit-bang driver for RPi4");

