#include <LPC21xx.h>
#include "types.h"
#include "defines.h"
#include "lcd_defines.h"
#include "lcd.h"
#include "delay.h"

static void LCD_WriteByte(u8 byte)
{
    // RW = 0 (write)
    IOCLR0 = (1U << LCD_RW);

    // put data on D0..D7 (P0.8–P0.15)
    WRITEBYTE(IOPIN0, LCD_DATA, byte);

    // EN high-low pulse
    IOSET0 = (1U << LCD_EN);
    delay_us(1);
    IOCLR0 = (1U << LCD_EN);
    delay_ms(2);
}

void WriteLCD(u8 byte)
{
    LCD_WriteByte(byte);
}

void CmdLCD(u8 cmdByte)
{
    // RS = 0 for command
    IOCLR0 = (1U << LCD_RS);
    LCD_WriteByte(cmdByte);
}

void CharLCD(u8 asciiVal)
{
    // RS = 1 for data
    IOSET0 = (1U << LCD_RS);
    LCD_WriteByte(asciiVal);
}

void StrLCD(s8 *str)
{
    while (*str)
        CharLCD((u8)*str++);
}

void U32LCD(u32 n)
{
    s8 buf[10];
    s32 i = 0;

    if (n == 0)
    {
        CharLCD('0');
        return;
    }

    while (n > 0)
    {
        buf[i++] = (n % 10) + '0';
        n /= 10;
    }

    while (--i >= 0)
        CharLCD(buf[i]);
}

void Init_LCD(void)
{
    // P0.8–P0.15, P0.16, P0.17, P0.18 as output
    IODIR0 |= (0xFFU << LCD_DATA) |
              (1U << LCD_RS) |
              (1U << LCD_EN) |
              (1U << LCD_RW);

    delay_ms(15);

    CmdLCD(MODE_8BIT_1LINE);
    delay_ms(4);
    CmdLCD(MODE_8BIT_1LINE);
    delay_us(100);
    CmdLCD(MODE_8BIT_1LINE);
    CmdLCD(MODE_8BIT_2LINE);
    CmdLCD(DSP_ON_CUR_OFF);
    CmdLCD(CLEAR_LCD);
    CmdLCD(SHIFT_CUR_RIGHT);
}

void SetCursor(u8 row, u8 col)
{
    u8 addr;

    switch (row)
    {
        case 1: addr = GOTO_LINE1_POS0 + col; break;
        case 2: addr = GOTO_LINE2_POS0 + col; break;
        case 3: addr = GOTO_LINE3_POS0 + col; break;
        case 4: addr = GOTO_LINE4_POS0 + col; break;
        default: addr = GOTO_LINE1_POS0 + col; break;
    }

    CmdLCD(addr);
}

// ---------------- Fuel Bar CGRAM (bike-style) ----------------

static void LCD_BuildCGRAM(u8 pos, const u8 *pattern)
{
    u8 i;
    CmdLCD(GOTO_CGRAM_START + (pos * 8));
    IOSET0 = (1U << LCD_RS); // data
    for (i = 0; i < 8; i++)
        LCD_WriteByte(pattern[i]);
}

void LCD_InitFuelBar(void)
{
    static const u8 bar0[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    static const u8 bar1[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F};
    static const u8 bar2[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x1F};
    static const u8 bar3[8] = {0x00,0x00,0x00,0x00,0x00,0x1F,0x1F,0x1F};
    static const u8 bar4[8] = {0x00,0x00,0x00,0x1F,0x1F,0x1F,0x1F,0x1F};

    LCD_BuildCGRAM(0, bar0);
    LCD_BuildCGRAM(1, bar1);
    LCD_BuildCGRAM(2, bar2);
    LCD_BuildCGRAM(3, bar3);
    LCD_BuildCGRAM(4, bar4);
}

// fuel_percent: 0..100, displayed as 5 bars horizontally
void LCD_DisplayFuel(u32 fuel_percent)
{
    u8 level_per_bar;  // 0..4
    u8 bars = 5;
    u8 i;
    u32 per_bar;

    if (fuel_percent > 100)
        fuel_percent = 100;

    per_bar = 100 / bars; // 20%

    for (i = 0; i < bars; i++)
    {
        u32 this_min = i * per_bar;
        u32 this_max = (i + 1) * per_bar;
        u32 local = 0;

        if (fuel_percent <= this_min)
            level_per_bar = 0;
        else if (fuel_percent >= this_max)
            level_per_bar = 4;
        else
        {
            local = fuel_percent - this_min; // 0..20
            level_per_bar = (u8)((local * 4) / per_bar); // 0..4
        }

        CharLCD(level_per_bar);  // prints CGRAM char 0..4
    }
}
