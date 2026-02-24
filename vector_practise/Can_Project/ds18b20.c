#include <LPC21xx.h>
#include "types.h"
#include "delay.h"

// DS18B20 connected to P0.19
#define DQ_PIN   19

static void DQ_Output(void)
{
    IODIR0 |= (1U << DQ_PIN);
}

static void DQ_Input(void)
{
    IODIR0 &= ~(1U << DQ_PIN);
}

static void DQ_Write(u8 bit)
{
    DQ_Output();
    IOCLR0 = (1U << DQ_PIN);    // pull low
    if (bit) delay_us(5);
    else     delay_us(60);
    IOSET0 = (1U << DQ_PIN);
}

static u8 DQ_Read(void)
{
    u8 b;
    DQ_Output();
    IOCLR0 = (1U << DQ_PIN);
    delay_us(3);

    DQ_Input();
    delay_us(10);

    b = (IOPIN0 >> DQ_PIN) & 1U;
    delay_us(55);

    return b;
}

static void ResetPulse(void)
{
    DQ_Output();
    IOCLR0 = (1U << DQ_PIN);
    delay_us(480);

    IOSET0 = (1U << DQ_PIN);
    delay_us(60);
}

static void WriteByte(u8 b)
{
    u8 i;
    for (i = 0; i < 8; i++)
    {
        DQ_Write(b & 1U);
        b >>= 1;
    }
}

static u8 ReadByte(void)
{
    u8 i, b = 0;
    for (i = 0; i < 8; i++)
    {
        b >>= 1;
        if (DQ_Read()) b |= 0x80;
    }
    return b;
}

void Init_DS18B20(void)
{
    DQ_Output();
    IOSET0 = (1U << DQ_PIN);
}

u32 ReadTemp(void)
{
    u8 LSB, MSB;
    u16 raw;

    ResetPulse();
    WriteByte(0xCC);      // skip ROM
    WriteByte(0x44);      // start temperature conversion

    delay_ms(750);        // DS18B20 max conversion time

    ResetPulse();
    WriteByte(0xCC);
    WriteByte(0xBE);

    LSB = ReadByte();
    MSB = ReadByte();

    raw = (u16)((MSB << 8) | LSB);
    return raw;           // 12-bit raw temp
}
