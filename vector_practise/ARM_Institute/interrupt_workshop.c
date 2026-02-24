#include <lpc21xx.h>
#include "types.h"
#include "pin_function_defines.h"
#include "pin_connect_block.h"
#include "lcd_defines.h"
#include "lcd.h"

#define EINT0_VIC_CHNO 14

void eint0_isr(void)__irq;
u32 gCount;
s8 str[20] = "Vector India";

main() {

	CfgPortPinFunc(0,1,EINT0_0_1);
//	VICIntSelect = 0; //{default is always 0 and hence it acts for irq interrupt)
	VICIntEnable = 1 << EINT0_VIC_CHNO;
	VICVectCntl0 = 1 << 5 | EINT0_VIC_CHNO;
	VICVectAddr0 = (u32)eint0_isr;
//	VICVectAddr; //(Not required bcoz it will be internally done)
//	EXTINT = 1 << 0; //(0 bcoz we are using EINT0)
//	EXTMODE = 1 << 0;
//	EXTPOLAR = 1 << 0;

	
	Init_LCD();	
	while(1) { //(this instruction is after the interrupt has been executed)
		CmdLCD(GOTO_LINE1_POS0);
		StrLCD(str);
	}

}

	void eint0_isr(void)__irq {
		CmdLCD(CLEAR_LCD);
		StrOppCaseLCD(str);
		delay_s(3);

		EXTINT = 1 << 0;
		VICVectAddr = 0;
	
		CmdLCD(CLEAR_LCD);		
	}
