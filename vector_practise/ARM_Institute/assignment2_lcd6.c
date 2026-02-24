#include "types.h"
#include "delay.h"
#include <lpc21xx.h>
#include "lcd_defines.h"
#include "lcd.h"

main() {
	u32 i,j,ch;
	Init_LCD();
	while(1) {
		StrLCD("A  D  H  O");
		for(i = 48;i<=122;i++) {
			CmdLCD(GOTO_LINE2_POS0);
			if((i >= 48 && i <= 57) || (i >= 65 && i <= 90) || (i >= 97 && i <= 122)) {
				ch = i;
				CharLCD(ch);
				StrLCD("  ");
				U32LCD(ch);
				StrLCD(" ");
				U32LCD(ch);
				StrLCD(" ");
				U32LCD(ch);
			}
			delay_s(2);
			for(j = 0;j<15;j++) {
				CharLCD(' ');
			}
		}
	}
}
		
