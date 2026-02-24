#ifndef __LCD_H__
#define __LCD_H__

#include "types.h"

void Init_LCD(void);
void WriteLCD(u8 byte);
void CmdLCD(u8 cmdByte);
void CharLCD(u8 asciiVal);
void StrLCD(s8 *str);
void U32LCD(u32 n);

void SetCursor(u8 row, u8 col);

void LCD_InitFuelBar(void);
void LCD_DisplayFuel(u32 fuel_percent);

// Convenience aliases
#define initilcd      Init_LCD
#define cmdlcd        CmdLCD
#define charlcd       CharLCD
#define strlcd        StrLCD
#define u32lcd        U32LCD
#define setcursor     SetCursor
#define display_fuel  LCD_DisplayFuel
#define buildcgram    LCD_InitFuelBar

#endif
