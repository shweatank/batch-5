#include <lpc21xx.h>
#include "types.h"
#include "lcd_defines.h"
#include "lcd.h"
#include "defines.h"
#include "delay.h"

int isPrime(u32 num) {
	u32 i;
	for(i = 2;i<((num/2)+1);i++) {
		if(num % i == 0) {
			return 0;
		}
	}
	return 1;
}

main() {
	while(1) {
		u32 i;
		Init_LCD();
		/* StrLCD("LCD-WORKSHOP");
		CmdLCD(GOTO_LINE2_POS0); */
		StrLCD("V24HE11-A13");
		CmdLCD(GOTO_LINE2_POS0);
		for(i = 2;i<256;i++) {
			if(isPrime(i)) {
				U32LCD(i);
				delay_s(1);
				CmdLCD(GOTO_LINE2_POS0);
			}
		}
	}				
}
