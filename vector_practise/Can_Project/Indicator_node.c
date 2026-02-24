// Indicator_node.c - Indicator node: receives CAN ID 2 and sweeps LEDs
#include <LPC21xx.h>
#include "types.h"
#include "defines.h"
#include "delay.h"
#include "can.h"
#include "can_defines.h"

#define LED_BASE  0   // LEDs on P0.0–P0.7

volatile u8 L = 0;
struct CAN_Frame rxFrame;

int main(void)
{
    u8 i;

    IODIR0 |= (0xFFU << LED_BASE);
    IOSET0  = (0xFFU << LED_BASE);

    Init_CAN1();

    while (1)
    {
        // If CAN data available
        if (C1GSR & RBS_BIT_READ)
        {
            CAN1_Rx(&rxFrame);

            if (rxFrame.ID == 2)
                L = rxFrame.Data1 & 0xFF;
        }

        if (L)
        {
            for (i = 0; i < 8 && L; i++)
            {
                // left sweep (MSB→LSB)
                WRITEBYTE(IOPIN0, LED_BASE, ~(1U << (7 - i)));
                delay_ms(100);
            }
        }
        else
        {
            IOSET0 = (0xFFU << LED_BASE);  // all off (assuming active low)
        }
    }
}
