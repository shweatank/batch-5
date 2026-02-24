#include "types.h"
#include <lpc21xx.h>
#include "lcd_defines.h"
#include "delay.h"
#include "lcd.h"

main() {
u32 count,i;
Init_LCD();
StrLCD("VECTOR");
CmdLCD(GOTO_LINE2_POS0);
for(count = 1;count<=5;count++) {
StrLCD("INSTITUTE");
delay_ms(900);
CmdLCD(GOTO_LINE2_POS0);
for(i = 0;i<16;i++) {
   CharLCD(' ');
}
CmdLCD(GOTO_LINE2_POS0);
delay_ms(100);
}
CmdLCD(CLEAR_LCD);
}
