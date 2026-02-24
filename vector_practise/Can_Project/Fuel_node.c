// Fuel_node.c - Fuel node: uses EINT1 to trigger fuel reading and sends CAN message
#include <LPC21xx.h>
#include "types.h"
#include "adc.h"
//#include "adc_defines.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "delay.h"
#include "can.h"
#include "can_defines.h"

volatile u8 fuel_trigger = 0;
struct CAN_Frame txFrame;

void EINT1_ISR(void) __irq
{
    fuel_trigger = 1;
    EXTINT = (1U << 1);
    VICVectAddr = 0;
}

static void Enable_EINT1(void)
{
    // P0.14 → EINT1 (FUNC3)
    PINSEL0 &= ~(3U << 28);
    PINSEL0 |=  (3U << 28);   // function 3

    VICIntEnable |= (1U << 15);
    VICVectCntl1  = 0x20 | 15;
    VICVectAddr1  = (u32)EINT1_ISR;

    EXTMODE |=  (1U << 1);   // edge triggered
}

int main(void)
{
    f32 volt;
    u32 fuel_percent;

    Init_ADC();
	Init_LCD();
    Init_CAN1();
    Enable_EINT1();

    while (1)
    {
//        if (fuel_trigger)
//        {
//            fuel_trigger = 0;

            volt = Read_ADC(0);

            // Convert voltage → percent (simple scaling)
            fuel_percent = (u32)((volt / 3.3f) * 100.0f);
            if (fuel_percent > 100) fuel_percent = 100;

//            txFrame.ID = 1;       // Fuel Node ID
//            txFrame.vbf.RTR = 0;
//            txFrame.vbf.DLC = 1;
//            txFrame.Data1 = fuel_percent & 0xFF;
//            txFrame.Data2 = 0;
//			
//            CAN1_Tx(txFrame);
			CmdLCD(GOTO_LINE1_POS0);
			U32LCD(fuel_percent);

            delay_ms(5000);
//        }
    }
}
