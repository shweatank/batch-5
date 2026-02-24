#include <LPC21xx.h>
#include "types.h"
#include "can.h"
#include "can_defines.h"

void Init_CAN1(void)
{
    // Configure P0.25 (TX1) and P0.26 (RD1) for CAN1
    // PINSEL1: bits18-19 for P0.25, bits20-21 for P0.26
    PINSEL1 &= ~((3U << 18) | (3U << 20));
    PINSEL1 |=  ((1U << 18) | (1U << 20));   // function 01

    // Reset CAN1 controller
    C1MOD = 1;

    // Bit timing
    C1BTR = BTR_LVAL;

    // Disable acceptance filtering (accept all)
    AFMR = 0x02;   // AccOff

    // Enable CAN1
    C1MOD = 0;
}

void CAN1_Tx(struct CAN_Frame frame)
{
    // Wait until transmit buffer 1 is free
    while ((C1GSR & TBS1_BIT_READ) == 0)
        ;

    // Standard ID
    C1TID1 = frame.ID & 0x7FF;

    // DLC & RTR
    C1TFI1 = ((u32)(frame.vbf.DLC & 0x0F) << 16);
    if (frame.vbf.RTR)
        C1TFI1 |= (1U << 30);

    C1TDA1 = frame.Data1;
    C1TDB1 = frame.Data2;

    // Start transmission from buffer 1
    C1CMR = TR_BIT_SET | STB1_BIT_SET;
}

void CAN1_Rx(struct CAN_Frame *frame)
{
    // Wait until a message is available
    while ((C1GSR & RBS_BIT_READ) == 0)
        ;

    frame->ID      = C1RID & 0x7FF;
    frame->vbf.DLC = (u8)((C1RFS >> 16) & 0x0F);
    frame->vbf.RTR = (u8)((C1RFS >> 30) & 0x01);
    frame->Data1   = C1RDA;
    frame->Data2   = C1RDB;

    // Release receive buffer
    C1CMR = RRB_BIT_SET;
}
