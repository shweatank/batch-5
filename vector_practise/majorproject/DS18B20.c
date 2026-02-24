#include <LPC21xx.h>
#include "delay.h"
#include "DS18B20.h"

/* 1-Wire data pin */
#define DQ_PIN   19
#define DQ_MASK (1 << DQ_PIN)

/* ---------- Low level helpers ---------- */
void DQ_OUT(void) { IODIR0 |=  DQ_MASK; }
void DQ_IN(void)  { IODIR0 &= ~DQ_MASK; }
void DQ_LOW(void) { IOCLR0 =  DQ_MASK; }
void DQ_HIGH(void){ IOSET0 =  DQ_MASK; }
int  DQ_READ(void){ return (IOPIN0 & DQ_MASK); }

/* ---------- DS18B20 reset ---------- */
unsigned char ResetDS18b20(void)
{
    unsigned char presence;

    DQ_OUT();
    DQ_HIGH();
    delayUs(5);

    DQ_LOW();                // reset pulse
    delayUs(480);

    DQ_IN();                 // release line
    delayUs(70);

    presence = !DQ_READ();   // DS18B20 pulls low

    delayUs(410);
    return presence;         // 1 = present, 0 = not present
}

/* ---------- Read 1 bit ---------- */
unsigned char ReadBit(void)
{
    unsigned char bit;

    DQ_OUT();
    DQ_LOW();
    delayUs(2);

    DQ_IN();
    delayUs(10);

    bit = DQ_READ() ? 1 : 0;
    delayUs(50);

    return bit;
}

/* ---------- Write 1 bit ---------- */
void WriteBit(unsigned char bit)
{
    DQ_OUT();
    DQ_LOW();
    delayUs(2);

    if (bit)
        DQ_HIGH();

    delayUs(60);
    DQ_HIGH();
}

/* ---------- Read 1 byte ---------- */
unsigned char ReadByte(void)
{
    unsigned char i, data = 0;

    for (i = 0; i < 8; i++)
    {
        data |= (ReadBit() << i);
        delayUs(5);
    }
    return data;
}

/* ---------- Write 1 byte ---------- */
void WriteByte(unsigned char data)
{
    unsigned char i;

    for (i = 0; i < 8; i++)
    {
        WriteBit(data & 0x01);
        data >>= 1;
        delayUs(5);
    }
}

/* ---------- Read temperature ---------- */
int ReadTemp(void)
{
    unsigned char lsb, msb;
    int temp;

    if (!ResetDS18b20())
        return 0x8000;   // sensor not present

    WriteByte(0xCC);     // Skip ROM
    WriteByte(0x44);     // Start conversion

    delayMs(750);        // wait for conversion (12-bit)

    ResetDS18b20();
    WriteByte(0xCC);     // Skip ROM
    WriteByte(0xBE);     // Read scratchpad

    lsb = ReadByte();
    msb = ReadByte();

    temp = (msb << 8) | lsb;
		//temp = 40;
    return temp;         // raw DS18B20 value
}
