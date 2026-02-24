#include <lpc21xx.h>
#include "lcd.h"
#include "lcd_defines.h"
#include "types.h"
#include "kpm_defines.h"
#include "kpm.h"

main() {
	u32 num;
	u8 key;
	Init_LCD();
	Init_KPM();
	StrLCD("KPM TEST");
	while(1) {
		CmdLCD(GOTO_LINE2_POS0);
		ReadNum(&num,&key);
	}
}