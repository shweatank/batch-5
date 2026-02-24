#include "types.h"

// Simple software delays (approximate, depend on compiler optimization and clock)
void delay_us(u32 d)
{
    volatile u32 i;
    while (d--)
    {
        for (i = 0; i < 10; i++) { __asm__("nop"); }
    }
}

void delay_ms(u32 d)
{
    volatile u32 i;
    while (d--)
    {
        for (i = 0; i < 1000; i++) { __asm__("nop"); }
    }
}
