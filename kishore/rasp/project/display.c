// ili9225_wiringpi.c
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdint.h>
#include <stdio.h>
#include"font8x8_basic.h"

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
#define DC_PIN  25
#define RST_PIN 24


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
    const uint8_t *bitmap = font8x8[c - 32];

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

void drawString(int x,int y,const char *str,uint16_t color)
{
	for(int i=0;str[i];i++)
	{
		drawChar(x,y,str[i],0xf800);
		x+=16;
	}
}
int main(int argc,char **argv)
{

	if(argc!=2){
		return 0;
	}

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
	lcd_fill(0xFFFF); delay(800); // White

	drawChar(0,0,'A',0x0000);delay(1000);
	int i=0;
		drawString(10,20,argv[1],0x0000);delay(1000);
	return 0;
}
