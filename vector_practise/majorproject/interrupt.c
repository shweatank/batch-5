#include <lpc21xx.h>
#include "types.h"
#include "pin_connect_block.h"
#include "pin_function_define.h"
#include "interrupt_defines.h"
#include "interrupt.h"

extern int l_flag, r_flag;

void interrupt_enable(void)
{
    /* Configure external interrupt pins */
    CfgPortPinFunc(0, 1, EINT0_0_1);   // P0.1  -> EINT0
    CfgPortPinFunc(0, 3, EINT1_0_3);   // P0.3  -> EINT1
    CfgPortPinFunc(0, 7, EINT2_0_7);   // P0.7  -> EINT2

    /* Enable EINT0, EINT1, EINT2 in VIC */
    VICIntEnable =
        (1 << EINT0_VIC_CHN0) |
        (1 << EINT1_VIC_CHN0) |
        (1 << EINT2_VIC_CHN0);

    /* Vector 0 : EINT0 */
    VICVectCntl0 = (1 << 5) | EINT0_VIC_CHN0;
    VICVectAddr0 = (u32)eint0_isr;

    /* Vector 1 : EINT1 */
    VICVectCntl1 = (1 << 5) | EINT1_VIC_CHN0;
    VICVectAddr1 = (u32)eint1_isr;

    /* Vector 2 : EINT2 */
    VICVectCntl2 = (1 << 5) | EINT2_VIC_CHN0;
    VICVectAddr2 = (u32)eint2_isr;

    /* Configure edge-triggered interrupts */
    EXTMODE = (1 << 0) | (1 << 1) | (1 << 2);
}

/* EINT0 ISR – Left indicator */
void eint0_isr(void) __irq
{
    l_flag = 1;
    r_flag = 0;

	
    EXTINT = (1 << 0);   // Clear EINT0 flag
    VICVectAddr = 0;    // End of ISR
}

/* EINT1 ISR – Right indicator */
void eint1_isr(void) __irq
{
    r_flag = 1;
    l_flag = 0;

    EXTINT = (1 << 1);   // Clear EINT1 flag
    VICVectAddr = 0;    // End of ISR
}

/* EINT2 ISR – Cancel / Hazard OFF */
void eint2_isr(void) __irq
{
    r_flag = 0;
    l_flag = 0;

    EXTINT = (1 << 2);   // Clear EINT2 flag
    VICVectAddr = 0;    // End of ISR
}
