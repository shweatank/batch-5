// ili9225_wiringpi.c
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdint.h>
#include <stdio.h>

#define SPI_CHANNEL 0
#define SPI_SPEED   8000000   // 8 MHz (stable)
/*
#define DC_PIN  6   // WiringPi pin 6 = BCM 24? (see note below)
#define RST_PIN 5   // WiringPi pin 5 = BCM 24? (change if needed)

*
IMPORTANT:
If using BCM numbering instead of WiringPi numbering,
replace wiringPiSetup() with wiringPiSetupGpio()
and use:*/
/*
#define DC_PIN  25
#define RST_PIN 24

const uint8_t font8x8[3][8] = {
    {0x18,0x3C,0x24,0x66,0x7E,0x66,0x66,0x00 }, // 'A'
    {0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00}, // 'B'
    {0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00}, // 'C'
};

void write16(uint16_t value)
{
    unsigned char buf[2];
    buf[0] = value >> 8;
    buf[1] = value & 0xFF;
    wiringPiSPIDataRW(SPI_CHANNEL, buf, 2);
}

void writeReg(uint16_t reg, uint16_t data)
{
    digitalWrite(DC_PIN, LOW);
    write16(reg);

    digitalWrite(DC_PIN, HIGH);
    write16(data);
}

void lcd_reset()
{
    digitalWrite(RST_PIN, HIGH); delay(5);
    digitalWrite(RST_PIN, LOW);  delay(20);
    digitalWrite(RST_PIN, HIGH); delay(50);
}

void lcd_init()
{
    lcd_reset();

    writeReg(0x0001, 0x011C);
    writeReg(0x0002, 0x0100);
    writeReg(0x0003, 0x1030);
    writeReg(0x0008, 0x0808);
    writeReg(0x000C, 0x0000);
    writeReg(0x000F, 0x0B01);

    writeReg(0x0010, 0x0A00);
    writeReg(0x0011, 0x1038);
    delay(50);
    writeReg(0x0012, 0x1121);
    writeReg(0x0013, 0x0063);
    writeReg(0x0014, 0x5A00);
    delay(50);

    writeReg(0x0007, 0x1017);
    delay(20);
}

void lcd_fill(uint16_t color)
{
    int x, y;

    writeReg(0x0036, 175);
    writeReg(0x0037, 0);
    writeReg(0x0038, 219);
    writeReg(0x0039, 0);
    writeReg(0x0020, 0);
    writeReg(0x0021, 0);

    digitalWrite(DC_PIN, LOW);
    write16(0x0022);
    digitalWrite(DC_PIN, HIGH);

    for (y = 0; y < 220; y++)
        for (x = 0; x < 176; x++)
            write16(color);
}

void drawPixel(int x, int y, uint16_t color)
{
    // Set X coordinate
    digitalWrite(DC_PIN, LOW);  // command
    write16(0x0020);            // GRAM X register
    digitalWrite(DC_PIN, HIGH); // data
    write16(x);                 // X coordinate

    //  Set Y coordinate
    digitalWrite(DC_PIN, LOW);  // command
    write16(0x0021);            // GRAM Y register
    digitalWrite(DC_PIN, HIGH); // data
    write16(y);                 // Y coordinate

    //  Write pixel color
    digitalWrite(DC_PIN, LOW);  // command
    write16(0x0022);            // GRAM write register
    digitalWrite(DC_PIN, HIGH); // data
    write16(color);             // RGB565 color
}

void drawChar(int x, int y, char c, uint16_t color)
{
    if(c < 32 || c > 127) return; // ignore non-printable
    const uint8_t *bitmap = font8x8[c - 'A'];

    for(int row = 0; row < 8; row++)
    {
        for(int col = 0; col < 8; col++)
        {
            if(bitmap[row] & (1 << (7 - col))) // bit 7 = leftmost
            {
                // draw 2x2 block instead of 1 pixel
                drawPixel(x + col*2, y + row*2, color);
                drawPixel(x + col*2+1, y + row*2, color);
                drawPixel(x + col*2, y + row*2+1, color);
                drawPixel(x + col*2+1, y + row*2+1, color);
            }
        }
    }
}


int main()
{
    wiringPiSetupGpio();   // Use BCM numbering

    pinMode(DC_PIN, OUTPUT);
    pinMode(RST_PIN, OUTPUT);

    if (wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) < 0)
    {
        printf("SPI Setup failed\n");
        return 1;
    }

    lcd_init();

    //lcd_fill(0xF800); delay(800); // Red
    //lcd_fill(0x07E0); delay(800); // Green
   // lcd_fill(0x001F); delay(800); // Blue
    //lcd_fill(0xFFFF); delay(800); // White

    drawPixel(0,0,0x0000);
   
    drawChar(0,0,'A',0x0000);delay(1000);
    return 0;
}*/
// ili9225_wiringpi_fixed.c
/*
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdint.h>
#include <stdio.h>

#define SPI_CHANNEL 0
#define SPI_SPEED   8000000

#define DC_PIN  25
#define RST_PIN 24

#define LCD_WIDTH  176
#define LCD_HEIGHT 220

// ===== 8x8 FONT (only A,B,C) =====
const uint8_t font8x8[3][8] = {
    {0x18,0x3C,0x24,0x66,0x7E,0x66,0x66,0x00}, // A
    {0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00}, // B
    {0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00}  // C
};

void write16(uint16_t value)
{
    uint8_t buf[2];
    buf[0] = value >> 8;
    buf[1] = value & 0xFF;
    wiringPiSPIDataRW(SPI_CHANNEL, buf, 2);
}

void writeCommand(uint16_t cmd)
{
    digitalWrite(DC_PIN, LOW);
    write16(cmd);
}

void writeData(uint16_t data)
{
    digitalWrite(DC_PIN, HIGH);
    write16(data);
}

void writeReg(uint16_t reg, uint16_t data)
{
    writeCommand(reg);
    writeData(data);
}

void lcd_reset()
{
    digitalWrite(RST_PIN, HIGH); delay(5);
    digitalWrite(RST_PIN, LOW);  delay(20);
    digitalWrite(RST_PIN, HIGH); delay(50);
}

void lcd_init()
{
    lcd_reset();

    writeReg(0x0001, 0x011C);
    writeReg(0x0002, 0x0100);
    writeReg(0x0003, 0x1030);
    writeReg(0x0008, 0x0808);
    writeReg(0x000C, 0x0000);
    writeReg(0x000F, 0x0B01);

    writeReg(0x0010, 0x0A00);
    writeReg(0x0011, 0x1038);
    delay(50);
    writeReg(0x0012, 0x1121);
    writeReg(0x0013, 0x0063);
    writeReg(0x0014, 0x5A00);
    delay(50);

    writeReg(0x0007, 0x1017);
    delay(20);
}

void setWindow(int x1, int y1, int x2, int y2)
{
    writeReg(0x0036, x2);
    writeReg(0x0037, x1);
    writeReg(0x0038, y2);
    writeReg(0x0039, y1);

    writeReg(0x0020, x1);
    writeReg(0x0021, y1);

    writeCommand(0x0022);
}

void lcd_fill(uint16_t color)
{
    setWindow(0,0,LCD_WIDTH-1,LCD_HEIGHT-1);
    digitalWrite(DC_PIN, HIGH);

    for(int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++)
        write16(color);
}

void drawChar(int x, int y, char c, uint16_t color, uint16_t bg)
{
    if(c < 'A' || c > 'C') return;

    const uint8_t *bitmap = font8x8[c - 'A'];

    // set 16x16 window (scaled 2x)
    setWindow(x, y, x+15, y+15);
    digitalWrite(DC_PIN, HIGH);

    for(int row = 0; row < 8; row++)
    {
        for(int sy = 0; sy < 2; sy++)   // vertical scale
        {
            for(int col = 0; col < 8; col++)
            {
                uint16_t pixelColor =
                    (bitmap[row] & (1 << (7-col))) ? color : bg;

                // horizontal scale
                write16(pixelColor);
                write16(pixelColor);
            }
        }
    }
}

int main()
{
    wiringPiSetupGpio();

    pinMode(DC_PIN, OUTPUT);
    pinMode(RST_PIN, OUTPUT);

    if (wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) < 0)
    {
        printf("SPI Setup failed\n");
        return 1;
    }

    lcd_init();

    lcd_fill(0xFFFF);   // white background

    drawChar(10, 10, 'A', 0x0000, 0xFFFF);
    drawChar(30, 10, 'D', 0x0000, 0xFFFF);
    drawChar(50, 10, 'C', 0x0000, 0xFFFF);

    while(1);
}*/






// ili9225_text_full.c

#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SPI_CHANNEL 0
#define SPI_SPEED   8000000

#define DC_PIN  25
#define RST_PIN 24

#define LCD_WIDTH  176
#define LCD_HEIGHT 220

// 8x8 font for ASCII 32–90 (space to Z)
// Only common characters included
const uint8_t font8x8[][8] = {

// Space (32)
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

// Numbers 0-9 (48-57)
{0x3C,0x66,0x6E,0x76,0x66,0x66,0x3C,0x00}, // 0
{0x18,0x38,0x18,0x18,0x18,0x18,0x3C,0x00}, // 1
{0x3C,0x66,0x06,0x1C,0x30,0x60,0x7E,0x00}, // 2
{0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00}, // 3
{0x0C,0x1C,0x3C,0x6C,0x7E,0x0C,0x0C,0x00}, // 4
{0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00}, // 5
{0x1C,0x30,0x60,0x7C,0x66,0x66,0x3C,0x00}, // 6
{0x7E,0x66,0x06,0x0C,0x18,0x18,0x18,0x00}, // 7
{0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00}, // 8
{0x3C,0x66,0x66,0x3E,0x06,0x0C,0x38,0x00}, // 9

// Letters A-Z (65-90)
{0x18,0x3C,0x66,0x66,0x7E,0x66,0x66,0x00}, // A
{0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00}, // B
{0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00}, // C
{0x78,0x6C,0x66,0x66,0x66,0x6C,0x78,0x00}, // D
{0x7E,0x60,0x60,0x7C,0x60,0x60,0x7E,0x00}, // E
{0x7E,0x60,0x60,0x7C,0x60,0x60,0x60,0x00}, // F
{0x3C,0x66,0x60,0x6E,0x66,0x66,0x3C,0x00}, // G
{0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x00}, // H
{0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00}, // I
{0x1E,0x0C,0x0C,0x0C,0x0C,0x6C,0x38,0x00}, // J
{0x66,0x6C,0x78,0x70,0x78,0x6C,0x66,0x00}, // K
{0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0x00}, // L
{0x63,0x77,0x7F,0x6B,0x63,0x63,0x63,0x00}, // M
{0x66,0x76,0x7E,0x7E,0x6E,0x66,0x66,0x00}, // N
{0x3C,0x66,0x66,0x66,0x66,0x66,0x3C,0x00}, // O
{0x7C,0x66,0x66,0x7C,0x60,0x60,0x60,0x00}, // P
{0x3C,0x66,0x66,0x66,0x6E,0x6C,0x36,0x00}, // Q
{0x7C,0x66,0x66,0x7C,0x78,0x6C,0x66,0x00}, // R
{0x3C,0x66,0x30,0x18,0x0C,0x66,0x3C,0x00}, // S
{0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x00}, // T
{0x66,0x66,0x66,0x66,0x66,0x66,0x3C,0x00}, // U
{0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00}, // V
{0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0x00}, // W
{0x66,0x66,0x3C,0x18,0x3C,0x66,0x66,0x00}, // X
{0x66,0x66,0x3C,0x18,0x18,0x18,0x18,0x00}, // Y
{0x7E,0x06,0x0C,0x18,0x30,0x60,0x7E,0x00}  // Z
};

#define FONT_OFFSET 32

void write16(uint16_t value)
{
    uint8_t buf[2];
    buf[0] = value >> 8;
    buf[1] = value & 0xFF;
    wiringPiSPIDataRW(SPI_CHANNEL, buf, 2);
}

void writeCommand(uint16_t cmd)
{
    digitalWrite(DC_PIN, LOW);
    write16(cmd);
}

void writeData(uint16_t data)
{
    digitalWrite(DC_PIN, HIGH);
    write16(data);
}

void writeReg(uint16_t reg, uint16_t data)
{
    writeCommand(reg);
    writeData(data);
}

void lcd_reset()
{
    digitalWrite(RST_PIN, HIGH); delay(5);
    digitalWrite(RST_PIN, LOW);  delay(20);
    digitalWrite(RST_PIN, HIGH); delay(50);
}

void lcd_init()
{
    lcd_reset();

    writeReg(0x0001, 0x011C);
    writeReg(0x0002, 0x0100);
    writeReg(0x0003, 0x1030);
    writeReg(0x0008, 0x0808);
    writeReg(0x0010, 0x0A00);
    writeReg(0x0011, 0x1038);
    delay(50);
    writeReg(0x0012, 0x1121);
    writeReg(0x0013, 0x0063);
    writeReg(0x0014, 0x5A00);
    delay(50);
    writeReg(0x0007, 0x1017);
    delay(20);
}

void setWindow(int x1,int y1,int x2,int y2)
{
    writeReg(0x0036, x2);
    writeReg(0x0037, x1);
    writeReg(0x0038, y2);
    writeReg(0x0039, y1);
    writeReg(0x0020, x1);
    writeReg(0x0021, y1);
    writeCommand(0x0022);
}

void lcd_fill(uint16_t color)
{
    setWindow(0,0,LCD_WIDTH-1,LCD_HEIGHT-1);
    digitalWrite(DC_PIN,HIGH);

    for(int i=0;i<LCD_WIDTH*LCD_HEIGHT;i++)
        write16(color);
}

void drawChar(int x,int y,char c,uint16_t color,uint16_t bg)
{
    if(c < 32 || c > 90) return;

    const uint8_t *bitmap = font8x8[c - FONT_OFFSET];

    setWindow(x,y,x+15,y+15);
    digitalWrite(DC_PIN,HIGH);

    for(int row=0;row<8;row++)
    {
        for(int sy=0;sy<2;sy++)
        {
            for(int col=0;col<8;col++)
            {
                uint16_t pix =
                (bitmap[row] & (1<<(7-col)))?color:bg;

                write16(pix);
                write16(pix);
            }
        }
    }
}

void drawString(int x,int y,const char *str,
                uint16_t color,uint16_t bg)
{
    int cx=x;
    int cy=y;

    while(*str)
    {
        if(*str=='\n')
        {
            cx=x;
            cy+=16;
            str++;
            continue;
        }

        drawChar(cx,cy,*str,color,bg);
        cx+=16;

        if(cx+16>LCD_WIDTH)
        {
            cx=x;
            cy+=16;
        }

        if(cy+16>LCD_HEIGHT) break;

        str++;
    }
}

int main()
{
    wiringPiSetupGpio();

    pinMode(DC_PIN,OUTPUT);
    pinMode(RST_PIN,OUTPUT);

    if(wiringPiSPISetup(SPI_CHANNEL,SPI_SPEED)<0)
    {
        printf("SPI failed\n");
        return 1;
    }

    lcd_init();

    lcd_fill(0xFFFF);

//    drawString(10,10,"HELLO",0x0000,0xFFFF);
    drawString(10,30,"WORLD 123",0xF800,0xFFFF);

    while(1);
}







