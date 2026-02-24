#include <LPC21xx.h>
#include "types.h"
#include "delay.h"
#include "defines.h"
#include "kpm_defines.h"
#include "lcd_defines.h"
#include "lcd.h"

u8 kpmLUT[4][4] = {
    {'1','2','3','/'},
    {'4','5','6','X'},
    {'7','8','9','-'},
    {'C','0','=','+'}
};

void Init_KPM(void) {
    WRITENIBBLE(IODIR1, ROWs_4, 0x0F);
    WRITENIBBLE(IOCLR1, ROWs_4, 0x0F);
}

u32 ColScan(void) {
    return (READNIBBLE(IOPIN1, COLs_4) == 0x0F) ? 1 : 0;
}

u32 RowCheck(void) {
    u32 r;
    for (r = 0; r < 4; r++) {
        WRITENIBBLE(IOPIN1, ROWs_4, ~(1 << r));
        if (ColScan() == 0) break;
    }
    WRITENIBBLE(IOPIN1, ROWs_4, 0x00);
    return r;
}

u32 ColCheck(void) {
    u32 c;
    for (c = 0; c < 4; c++) {
        if (READBIT(IOPIN1,(COL0+c)) == 0) break;
    }
    return c;
}

u32 KeyScan(void) {
    u32 r, c;
    u8 keyV;
    while (ColScan());
    delay_ms(20);
    r = RowCheck();
    c = ColCheck();
    while (ColScan() == 0);
    keyV = kpmLUT[r][c];
    return keyV;
}

void ReadNum(s32 *num, u8 *key)
{
	*num = -1;
	while (1) {
		*key = KeyScan();
		loop: if(*key >= '0' && *key <= '9') {
						if(*num == -1) *num = 0;
						*num = (*num * 10) + (*key - '0');
						CmdLCD(GOTO_LINE2_POS0);
						U32LCD(*num);
						while (ColScan() == 0);
				  }
					else if(*key == 'X') {
						while(*key == 'X') {
							*num = *num / 10;
							CmdLCD(GOTO_LINE2_POS0);
							StrLCD("                ");
							CmdLCD(GOTO_LINE2_POS0);
							U32LCD(*num);
							while (ColScan() == 0);
							*key = KeyScan();
							if(*key != 'X') goto loop;
						}
					}
					else if(*key == '=')
					{
						break;
					}
	}
}
