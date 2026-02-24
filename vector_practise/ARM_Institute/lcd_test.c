//lcd_test.c
#include "lcd.h"
#include "lcd_defines.h"
#include "delay.h"

u8 cgramLUT[16]=
{
	0x1d,0x05,0x05,0x1f,0x14,0x14,0x17,0x00,
	0x15,0x15,0x15,0x1f,0x04,0x04,0x04,0x00
};

main()
{
	Init_LCD();
	CharLCD('A');
	StrLCD(" V24HE11-ARM ");
	CmdLCD(GOTO_LINE2_POS0);
	U32LCD(1234567890);
	delay_ms(1000);
	CmdLCD(CLEAR_LCD);
	S32LCD(-65534);
	delay_ms(1000);
	CmdLCD(CLEAR_LCD);
	F32LCD(-65.123534,3);
	CmdLCD(GOTO_LINE2_POS0);
	F32LCD(123.456789,3);
	delay_ms(1000);
	CmdLCD(CLEAR_LCD);
	BuildCGRAM(cgramLUT,16);
	CharLCD(0);
	CharLCD(1);
	while(1);
}
