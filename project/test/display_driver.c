#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/init.h>

#define SCK 11+512   // BCM11
#define MOSI 10+512  // BCM10
#define DC 25+512    // BCM25
#define RST 24+512   // BCM24
#define CS 8+512     // BCM8 (CE0)

/* * ILI9225 Constants 
 * Screen is 176x220
 */
#define ILI9225_WIDTH  176
#define ILI9225_HEIGHT 220

// Bit-bang 16 bits to the display
static void spi_write16(u16 val)
{
    int i;
    // CS must be LOW during the entire 16-bit transfer
    gpio_set_value(CS, 0);
    for (i = 15; i >= 0; i--) {
        gpio_set_value(MOSI, (val >> i) & 0x01);
        udelay(1); // Small delay for RPi4 speed
        gpio_set_value(SCK, 1);
        udelay(1);
        gpio_set_value(SCK, 0);
        udelay(1);
    }
    gpio_set_value(CS, 1);
}

static void write_command(u16 cmd)
{
    gpio_set_value(DC, 0); // DC LOW = Command
    spi_write16(cmd);
}

static void write_data(u16 data)
{
    gpio_set_value(DC, 1); // DC HIGH = Data
    spi_write16(data);
}

// Set the addressable "window" for drawing
static void set_window(u16 x0, u16 y0, u16 x1, u16 y1)
{
    write_command(0x0036); write_data(x1); // Horizontal End
    write_command(0x0037); write_data(x0); // Horizontal Start
    write_command(0x0038); write_data(y1); // Vertical End
    write_command(0x0039); write_data(y0); // Vertical Start
    
    write_command(0x0020); write_data(x0); // RAM Address Set 1
    write_command(0x0021); write_data(y0); // RAM Address Set 2
    write_command(0x0022);                 // Prepare for RAM write
}

static void lcd_reset(void)
{
    gpio_set_value(RST, 1); mdelay(10);
    gpio_set_value(RST, 0); mdelay(50);
    gpio_set_value(RST, 1); mdelay(50);
}

static void lcd_init(void)
{
    lcd_reset();

    // ILI9225 Standard Initialization Sequence
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

static void lcd_fill(u16 color)
{
    int i;
    set_window(0, 0, 175, 219);
    gpio_set_value(DC, 1); // Set to data mode once for bulk transfer
    for (i = 0; i < (ILI9225_WIDTH * ILI9225_HEIGHT); i++) {
        spi_write16(color);
    }
}

// Draw a simple box (for the 'HI' pattern)
static void draw_rect(u16 x, u16 y, u16 w, u16 h, u16 color)
{
    int i;
    set_window(x, y, x + w - 1, y + h - 1);
    gpio_set_value(DC, 1);
    for (i = 0; i < (w * h); i++) {
        spi_write16(color);
    }
}

static void draw_HI(void)
{
    u16 yellow = 0xFFE0;
    
    // Letter H
    draw_rect(20, 40, 5, 40, yellow);  // Left bar
    draw_rect(40, 40, 5, 40, yellow);  // Right bar
    draw_rect(25, 58, 15, 5, yellow);  // Middle bar
    
    // Letter I
    draw_rect(60, 40, 5, 40, yellow);  // Stem
}

static int __init display_driver_init(void)
{
    int ret;

    // Request and configure GPIOs
    ret = gpio_request_one(SCK, GPIOF_OUT_INIT_LOW, "SCK"); if(ret) return ret;
    ret = gpio_request_one(MOSI, GPIOF_OUT_INIT_LOW, "MOSI"); if(ret) return ret;
    ret = gpio_request_one(DC, GPIOF_OUT_INIT_LOW, "DC"); if(ret) return ret;
    ret = gpio_request_one(RST, GPIOF_OUT_INIT_HIGH, "RST"); if(ret) return ret;
    ret = gpio_request_one(CS, GPIOF_OUT_INIT_HIGH, "CS"); if(ret) return ret;

    pr_info("ILI9225: Starting Init...\n");

    lcd_init();
    lcd_fill(0x001F); // Blue background
    draw_HI();

    pr_info("ILI9225: Display Updated\n");
    return 0;
}

static void __exit display_driver_exit(void)
{
    // Clean up
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
MODULE_AUTHOR("team2");
MODULE_DESCRIPTION("Fixed ILI9225 bit-bang driver");
