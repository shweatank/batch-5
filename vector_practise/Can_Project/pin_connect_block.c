#include <LPC21xx.h>
#include "types.h"
#include "pin_connect_block.h"

void CfgPortPinFunc(u8 port, u8 pin, u8 func)
{
    if (port != 0) return; // only port 0 supported here

    if (pin < 16)
    {
        u32 shift = pin * 2;
        PINSEL0 &= ~(3U << shift);
        PINSEL0 |= ((u32)(func & 0x3U) << shift);
    }
    else if (pin < 32)
    {
        u32 shift = (pin - 16U) * 2U;
        PINSEL1 &= ~(3U << shift);
        PINSEL1 |= ((u32)(func & 0x3U) << shift);
    }
}
