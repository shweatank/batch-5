#include <lpc21xx.h>
#include "types.h"
#include "delay.h"

#define LED_BASE   0          // P0.0 – P0.7
#define LEFT_MASK  (0x0F << LED_BASE)   // P0.0–P0.3
#define RIGHT_MASK (0xF0 << LED_BASE)   // P0.4–P0.7

extern u8 l, r;

void indicatorBlink(void)
{
    u8 i;

    /* Configure P0.0–P0.7 as output */
    IODIR0 |= (0xFF << LED_BASE);

    /* Turn OFF all LEDs initially */
    IOSET0 = (0xFF << LED_BASE);

    /* No indicator */
    if (l == 0 && r == 0)
    {
        IOSET0 = (0xFF << LED_BASE);
    }

    /* Left indicator ? lower 4 LEDs blink */
    else if (l == 1 && r == 0)
    {
        for (i = 0; i < 4 && l == 1; i++)
        {
            IOCLR0 = (1 << (LED_BASE + i));   // LED ON
            delayMs(300);
            IOSET0 = (1 << (LED_BASE + i));   // LED OFF
        }
    }

    /* Right indicator ? upper 4 LEDs blink */
    else if (l == 0 && r == 1)
    {
        for (i = 4; i < 8 && r == 1; i++)
        {
            IOCLR0 = (1 << (LED_BASE + i));   // LED ON
            delayMs(300);
            IOSET0 = (1 << (LED_BASE + i));   // LED OFF
        }
    }
}
