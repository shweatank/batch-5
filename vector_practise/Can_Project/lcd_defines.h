#ifndef __LCD_DEFINES_H__
#define __LCD_DEFINES_H__

// HD44780 Commands
#define CLEAR_LCD            0x01
#define RET_CUR_HOME         0x02
#define SHIFT_CUR_RIGHT      0x06
#define DSP_OFF              0x08
#define DSP_ON_CUR_OFF       0x0C
#define DSP_ON_CUR_ON        0x0E
#define DSP_ON_CUR_BLINK     0x0F
#define MODE_8BIT_1LINE      0x30
#define MODE_8BIT_2LINE      0x38
#define GOTO_LINE1_POS0      0x80
#define GOTO_LINE2_POS0      0xC0
#define GOTO_LINE3_POS0      0x94
#define GOTO_LINE4_POS0      0xD4
#define GOTO_CGRAM_START     0x40

// LPC2129 LCD pin mapping (8-bit mode)
// P0.8–P0.15: D0–D7
// P0.16: RS
// P0.17: EN
// P0.18: RW
#define LCD_DATA   8
#define LCD_RS    16
#define LCD_EN    17
#define LCD_RW    18

#endif
