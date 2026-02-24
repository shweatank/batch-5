#include "types.h"
#include "delay.h"
#include <lpc21xx.h>
#include "lcd_defines.h"
#include "lcd.h"

main() {
	u32 i,j,ch;
	Init_LCD();
	while(1) {
		for(i = 48;i<=122;i++) {
			if((i >= 48 && i <= 57) || (i >= 65 && i <= 90) || (i >= 97 && i <= 122)) {
				CharLCD(i);
				CmdLCD(GOTO_LINE2_POS0);
				BinaryInLCD(i);
				delay_ms(500);
				CmdLCD(CLEAR_LCD);
			}
		}
	}
}
		