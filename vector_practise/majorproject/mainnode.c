#include <lpc21xx.h>
#include "delay.h"
#include "lcd_defines.h"
#include "lcd.h"
#include "interrupt.h"
#include "can.h"
#include "can_defines.h"
#include "DS18B20.h"

volatile int l_flag = 0, r_flag = 0;

int temp;
unsigned int tp, tpd;

CANF txf;
CANF rxf;

u8 cgramLUT[48] =
{
    0x01, 0x03, 0x07, 0x0F, 0x0F, 0x07, 0x03, 0x01,
    0x00, 0x00, 0x00, 0x1F, 0x1F, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x1F, 0x1F, 0x00, 0x00, 0x00,
    0x10, 0x18, 0x1C, 0x1E, 0x1E, 0x1C, 0x18, 0x10
};

int main(void)
{
    Init_LCD();
    Init_CAN1();
    interrupt_enable();
    // BuildCGRAM(cgramLUT, 48);

    txf.ID        = 1;
    txf.vbf.RTR   = 0;
    txf.vbf.DLC   = 1;
	
		//IODIR0 |= 1 << 4|1 << 5|1 << 6;		//for debug

    CmdLCD(GOTO_LINE1_POS0 + 3);
    StrLCD("CAR INFO");

    while (1)
    {
        /* Read temperature from DS18B20 */
        temp = ReadTemp();

        tp  = temp >> 4;                     // Integer part
        tpd = (temp & 0x08) ? 0x35 : 0x30;    // Fractional part

        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("T:");
        CmdLCD(GOTO_LINE2_POS0 + 2);

        U32LCD(tp);
        CharLCD('.');
        U32LCD(tpd);
        CharLCD(223);
        CharLCD('c');

        /* CAN Receive */
        if (C1GSR & RBS_BIT_READ)
        {
            CAN1_Rx(&rxf);
            if (rxf.ID == 2)
            {
                CmdLCD(GOTO_LINE2_POS0 + 8);
                StrLCD("F:");

                if (rxf.Data1 <= 10)
                    StrLCD("LOW");
                else
                    U32LCD(rxf.Data1);

                CmdLCD(GOTO_LINE2_POS0 + 14);
                CmdLCD(0x25);
            }
        }

        /* Left indicator */
        if (l_flag == 1)
        {
            CmdLCD(GOTO_LINE1_POS0 + 1);
            CharLCD(0);
            CharLCD(1);
						
            txf.Data1 = 0x0A;
            CAN1_Tx(txf);
					

//					IOCLR0 = 1<<4;
//					delayMs(500);
//					IOSET0 = 1<<4;

            l_flag = 0;
        }

        /* Right indicator */
        if (r_flag == 1)
        {
            CmdLCD(GOTO_LINE1_POS0 + 12);
            CharLCD(2);
            CharLCD(3);

            txf.Data1 = 0x0B;
            CAN1_Tx(txf);

//					IOCLR0 = 1 << 5;
//					delayMs(500);
//					IOSET0 = 1 << 5;
					
            r_flag = 0;
        }

        /* No indicator */
        if (r_flag == 0 && l_flag == 0)
        {
            CmdLCD(GOTO_LINE1_POS0 + 12);
            CharLCD(2);
            CharLCD(3);

//					IOCLR0 = 1 << 6;
//					delayMs(500);
//					IOSET0 = 1 << 6;
					
            txf.Data1 = 0x0C;
            CAN1_Tx(txf);
        }
    }
}
