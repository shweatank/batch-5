#include "types.h"
#include "seg_defines.h"
#include "delay.h"

#include <lpc21xx.h>

#define OUTPUT 1
#define SETPINMODE(REGISTER_NAME,PINNO,OUTPUT) REGISTER_NAME |= OUTPUT << PINNO
#define WRITEBYTE(REGISTER_NAME,PINNO,DATA) \
				REGISTER_NAME = ((REGISTER_NAME & ~(0XFF << PINNO)) | (DATA << PINNO))

u8 segLUT[] = {0xC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0x80,0X90};

void init_7seg() {
	u32 i;
	//cfg p0.8 to p0.15 as gpio op ports
	for(i = 0;i<8;i++) {
		SETPINMODE(IODIR0,CA7_SEGA_PIN+i,OUTPUT);
	}
}

void display_7seg(u32 num) {
	WRITEBYTE(IOPIN0,CA7_SEGA_PIN,segLUT[num]);
}

void init_mux_7seg() {
	// cfg po.8 to p0.15 and p0.16,p0.17 for dsel1,dsel2 as gpio op pins
	IODIR0 |= 0xff << CA7_SEGA_PIN | 1 << DSEL1 | 1 << DSEL2;
}

void display_mux_7seg(u32 num) {
	//write the data for 1st 7seg
	WRITEBYTE(IOPIN0,CA7_SEGA_PIN,segLUT[num/10]);

	//set the dsel1 pin which is connected to seg1
	IOSET0 = 1 << DSEL1;
	delay_ms(1);

	//clear the dsel1 pin
	IOCLR0 = 1 << DSEL1;

	//repeat the above operations for 2nd 7seg
	WRITEBYTE(IOPIN0,CA7_SEGA_PIN,segLUT[num%10]);
	IOSET0 = 1 << DSEL2;
	delay_ms(1);
	IOCLR0 = 1 << DSEL2;
}
