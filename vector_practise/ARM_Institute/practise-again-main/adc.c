#include <lpc21xx.h>
#include "lcd.c"

#define fosc	12000000
#define cclk	(fosc*5)
#define pclk	(cclk/4)
#define addclk	3000000
#define clkdiv	((pclk/addclk)-1)


void read_adc(int chno,int *digital,float * analog) {
	PINSEL1 |= 0x15400000;
	ADCR = 0;	//clearing the previous inputs
	ADCR |= 1 << chno | clkdiv << 8 | 1 << 21 | 1 << 24;
	delay_us(3);
	while(((ADDR >> 31) & 1) == 0);
	ADCR &= ~(1 << 24);
	*digital = ((ADDR >> 6) & 1023);
	*analog = (*digital * (3.3/1023));
}
	
	