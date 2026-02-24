//lcd.c
#include "rtc_defines.h"
#ifndef _LPC2148_
#include <lpc21xx.h>
#else
#include <lpc214x.h>
#endif

#include <string.h>
#include "types.h"
#include "defines.h"
#include "lcd_defines.h"
#include "lcd.h"
#include "delay.h"

void WriteLCD(u8 byte)
{
	//select write operation
	IOCLR0=1<<LCD_RW;
	//write byte to lcd data pins
	WRITEBYTE(IOPIN0,LCD_DATA,byte);
	//provide high to low pulse
	IOSET0=1<<LCD_EN;
	delay_us(1);
	IOCLR0=1<<LCD_EN;
	delay_ms(2);
}

void CmdLCD(u8 cmdByte)
{
	//set rs pin for cmd/inst reg
  IOCLR0=1<<LCD_RS;
  //write cmd byte to cmd reg
  WriteLCD(cmdByte); 	
}

void Init_LCD(void)
{
	//cfg p0.8(d0) to p0.15(d7),
	//p0.16(rs),p0.17(rw),p0.18(en) 
	//as gpio output pins
	IODIR0|=((0xFF<<LCD_DATA)|
	         (1<<LCD_RS)|(1<<LCD_RW)|(1<<LCD_EN));
	
	//power on delay
	delay_ms(15);
	CmdLCD(MODE_8BIT_1LINE);
	delay_ms(4);
	delay_us(100);
	CmdLCD(MODE_8BIT_1LINE);
	delay_us(100);
	CmdLCD(MODE_8BIT_1LINE);
	CmdLCD(MODE_8BIT_2LINE);
	CmdLCD(DSP_ON_CUR_OFF);
	CmdLCD(CLEAR_LCD);
	CmdLCD(SHIFT_CUR_RIGHT);
}

void CharLCD(u8 asciiVal)
{
	//sel data reg
	IOSET0=1<<LCD_RS;
	//write ascii value via data reg to ddram
	WriteLCD(asciiVal);
}

void StrLCD(s8 *str)
{
	while(*str)
		CharLCD(*str++);
}

void U32LCD(u32 n)
{
	s32 i=0;
	u8 a[10];
	
	if(n==0)
	{
		CharLCD('0');
	}
	else
	{
	  while(n>0)
    {
		  a[i++]=(n%10)+48;
      n/=10;			
    }
    for(--i;i>=0;i--)
      CharLCD(a[i]);		
	}
}

void S32LCD(s32 num)
{
	if(num<0)
	{
		CharLCD('-');
		num=-num;
	}
	U32LCD(num);
}

void F32LCD(f32 fnum,u32 nDP)
{
	u32 num,i;
	if(fnum<0.0)
	{
		CharLCD('-');
		fnum=-fnum;
	}
	num=fnum;
	U32LCD(num);
	CharLCD('.');
	for(i=0;i<nDP;i++)
	{
		fnum=(fnum-num)*10;
		num=fnum;
		CharLCD(num+48);
	}
}

void BuildCGRAM(u8 *p,u8 nBytes,u32 cgram_pos)
{
	u32 i;
	//point to cgram start
	CmdLCD(GOTO_CGRAM_START+cgram_pos);
	//select data reg
	IOSET0=1<<LCD_RS;
	
	for(i=0;i<nBytes;i++)
	{
		//write to cgram vi data reg
		WriteLCD(p[i]);
	}
}

void CGRAM_Symbols() {
	u8 up_arrow[] = {0x04,0x0E,0x15,0x04,0x04,0x04,0x04,0x04};
	u8 down_arrow[] = {0x04,0x04,0x04,0x04,0x04,0x15,0x0E,0x04};
	u8 alarm_symbol[] = {0x04,0x0E,0x0E,0x0E,0x1F,0x00,0x04,0x00};
	
	BuildCGRAM(up_arrow,8,0);
	BuildCGRAM(down_arrow,8,8);
	BuildCGRAM(alarm_symbol,8,16);
}

void Display_temperature(u32 temperature) {
	CharLCD(' ');
	U32LCD(temperature);
	WriteLCD(0xEB);
	CharLCD('C');
}

void StrOppCaseLCD(s8 *str) {
	s8 ch;
	u32 i = 0;
	while(str[i]) {
		ch = str[i];
		if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
			ch = (32 ^ ch);
		}
		CharLCD(ch);
		i++;
	}
}

void BinaryInLCD(u32 num) {
	s32 i = 7;
	while(i >= 0) {
		if((num >> i)&1) {
			CharLCD('1');
		}
		else {
			CharLCD('0');
		}
		i--;
	}
}

void StrFirstAndLastCharOppCase(s8 *str) {
	u8 first = str[0];
	u8 last = str[strlen((char *)str) - 1];
	if((first >= 'A' && first <= 'Z') || (first >= 'a' && last <= 'z')) {
		str[0] ^= 32;
	}
	if((last >= 'A' && last <= 'Z') || (last >= 'a' && last <= 'z')) {
		str[strlen((char *)str) - 1] ^= 32;
	}
}
		 
