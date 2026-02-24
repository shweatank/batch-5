#include "types.h"
#include "pin_function_defines.h"
#include "pin_connect_block.h"
#include "lcd_defines.h"
#include "lcd.h"

#define EINT0_VIC_CHNO 14
#define EINT0_LED 16 //p1.16

void eint0_isr(void)__irq;
u32 gCount;

main() {

	cfgProtPinFunc(0,1,EINT0_0_1);
//	VICIntSelect = 0; //{default is always 0 and hence it acts for irq interrupt)
	VICIntEnable = 1 << EINT0_VIC_CHNO;
	VICVectCntl0 = 1 << 5 | 1 << EINT0_VIC_CHNO;
	VICVectAddr0 = (u32)eint0_isr;
//	VICVectAddr; //(Not required bcoz it will be internally done)
	EXTINT = 1 << 0 //(0 bcoz we are using EINT0)
	EXTMODE = 1 << 0;
	EXTPOLAR = 0 << 0;
	
	while(1) { //(this instruction is after the interrupt has been executed)
		gCount++;
	}

	void eint0_isr(void)__irq 
