#include <lpc21xx.h>
#include "lcd.h"
#include "lcd_defines.h"
#include "types.h"
#include "kpm_defines.h"
#include "kpm.h"

main() {
	u32 keyV;
	Init_LCD();
	Init_KPM();
	StrLCD("KPM TEST");
	while(1) {
		CmdLCD(GOTO_LINE2_POS0);
		keyV = KeyScan();
		U32LCD(keyV);
		while(ColScan() == 0);
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("  ");
	}
}