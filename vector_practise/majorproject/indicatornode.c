#include <lpc21xx.h>
#include "types.h"
#include "can.h"
#include "can_defines.h"
#include "indicatornode.h"

volatile u8 l = 0, r = 0;

int main(void)
{
    CANF rxf;

    Init_CAN1();

    while (1)
    {
        if (C1GSR & RBS_BIT_READ)
        {
            CAN1_Rx(&rxf);

            if (rxf.ID == 1)
            {
                if (rxf.Data1 == 0x0A)
                {
                    l = 1;
                }
                else if (rxf.Data1 == 0x0B)
                {
                    r = 1;
                }
                else if (rxf.Data1 == 0x0C)
                {
                    l  = 0;
                    r = 0;
                }
            }
        }

        indicatorBlink();
    }
}
