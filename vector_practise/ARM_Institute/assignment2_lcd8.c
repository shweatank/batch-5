#include "types.h"
#include <lpc21xx.h>
#include "lcd.h"
#include "lcd_defines.h"
#include "defines.h"

#define SW_AL 1
#define SW_AH 2

main() {
	s8 str[] = "vector india";
	u32 flag = 0;
	IODIR0 |= 1 << SW_AL;
	IODIR0 |= 1 << SW_AH;
	IOSET0 =  1 << SW_AL;
	Init_LCD();
	while(1) {
		while(READBIT(IOPIN0,SW_AL) == 0) {
			StrLCD(str);
			flag = 1;
		}
		if(flag && READBIT(IOPIN0,SW_AL) == 1) {
			CmdLCD(GOTO_LINE2_POS0);
			StrFirstAndLastCharOppCase(str);
		}
		flag = 0;
		CmdLCD(CLEAR_LCD);
	}
}