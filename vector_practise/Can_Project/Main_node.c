// Main_node.c - Main CAN node: reads temperature, receives fuel, sends indicator command
#include <LPC21xx.h>
#include "types.h"
#include "defines.h"
#include "delay.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "can.h"
#include "can_defines.h"
#include "ds18b20.h"

#define EINT0_PIN 16   // P0.16 → EINT0 (indicator request)

volatile u8 indicator_flag = 0;  // 0 = off, 1 = left (example)
struct CAN_Frame txFrame, rxFrame;

void EINT0_ISR(void) __irq
{
    // Toggle indicator on each press
    indicator_flag ^= 1;
    EXTINT = (1U << 0);
    VICVectAddr = 0;
}

static void Enable_EINT0(void)
{
    // P0.16 → EINT0, FUNC1
    PINSEL1 &= ~(3U << 0);
    PINSEL1 |=  (1U << 0);

    VICIntEnable |= (1U << 14);
    VICVectCntl0  = 0x20 | 14;
    VICVectAddr0  = (u32)EINT0_ISR;

    EXTMODE |= (1U << 0);   // edge triggered
}

int main(void)
{
    u32 raw_temp;
    u32 tempC;
    u32 fuel_percent = 0;

    Init_LCD();
    LCD_InitFuelBar();
    Init_DS18B20();
    Init_CAN1();
    Enable_EINT0();

    CmdLCD(CLEAR_LCD);
    SetCursor(1,0); StrLCD("ENGINE STATUS");

    while (1)
    {
        // -------- TEMPERATURE --------
        raw_temp = ReadTemp();     // 12-bit raw
        tempC = raw_temp >> 4;     // °C integer

        SetCursor(2,0);
        StrLCD("TEMP: ");
        U32LCD(tempC);
        StrLCD("C   ");

        // ---------- FUEL RECEIVE ----------
        if (C1GSR & RBS_BIT_READ)
        {
            CAN1_Rx(&rxFrame);
            if (rxFrame.ID == 1)   // fuel message
                fuel_percent = rxFrame.Data1 & 0xFF;
        }

        SetCursor(3,0);
        StrLCD("FUEL: ");
        U32LCD(fuel_percent);
        StrLCD("%  ");

        SetCursor(4,0);
        LCD_DisplayFuel(fuel_percent);

        // ---------- SEND INDICATOR ----------
        if (indicator_flag == 1)
        {
            txFrame.ID = 2;       // LEFT indicator ON
            txFrame.vbf.DLC = 1;
            txFrame.vbf.RTR = 0;
            txFrame.Data1 = 1;
            txFrame.Data2 = 0;
            CAN1_Tx(txFrame);

            SetCursor(2,14);
            StrLCD("<-");
        }
        else
        {
            txFrame.ID = 2;       // LEFT indicator OFF
            txFrame.vbf.DLC = 1;
            txFrame.vbf.RTR = 0;
            txFrame.Data1 = 0;
            txFrame.Data2 = 0;
            CAN1_Tx(txFrame);

            SetCursor(2,14); StrLCD("  ");
        }

        delay_ms(200);
    }
}
