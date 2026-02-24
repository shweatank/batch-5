#include <lpc21xx.h>
#include "lcd.c"

void eint1_isr(void)__irq {
	cmdlcd(0x01);
	delay_ms(500);
	char_lcd('I');
	EXTINT = 1 << 1;
	VICVectAddr = 0;
}

int main() {
	init_lcd();
	
	PINSEL0 = 0x000000C0;
	//IOSET0 = 1 << 3;
	
	VICIntSelect = 0; //default status and we keeping it 0 bcoz we are using irq interrupts
	VICIntEnable = 1 << 15; //15 is the interrupt channel number for external interrupt 1
	
	EXTINT = 0 << 1; // 1 bit leftshift bcoz the 1st bit in this represents ext int1
	EXTMODE = 0 << 1;
	EXTPOLAR = 0 << 1;
	
	VICVectCntl0 = 15 | (1 << 5);
	VICVectAddr0 = (int)eint1_isr;
	
		cmdlcd(0x80);
		char_lcd('O');
	while(1);
}
