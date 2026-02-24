#include <lpc21xx.h>
#include "kpm.c"

#define tx	0
#define rx 	1

//void delay_ms(int num) {
//	num *= 12000;
//	while(num--);
//}

//void delay_us(int num) {
//	num *= 12;
//	while(num--);
//}

void init_uart() {
	//cfg tx as op and rx as ip pins
	IODIR0 |= 1 << tx;
	IOSET0 = 1 << tx;	//we are setting high bcoz to start uart we need to change the status of pin from high to low
//	IODIR0 |= 0 << rx;
//	IOSET0 = 1 << rx;
}

void uart_tx(char data) {
	int i;
	IOCLR0 = 1 << tx;	//high to low turns on the uart and start bit is sent
	delay_us(104);	// as per 9600 baud rate
	for(i = 0;i < 8;i++) {
		IOPIN0 = (IOPIN0 & ~(1 << tx)) | (((data >> i) & 1) << tx);	//clearing the previous bit	//sending each bit from lsb to msb
		delay_us(104);
	}
	IOSET0 = 1 << tx;	//sending the stop bit
}

char uart_rx() {
	int i;
	char rdata;
	while(((IOPIN0 >> rx) & 1) == 1);	//wait umtil the start bit is detected;
	delay_us(52);	//wait for half of the baud rate bit period
	for(i = 0;i<8;i++) {
		delay_us(104);
		rdata = (rdata &~(1 << i)) | ((IOPIN0 >> rx) & 1) << i;
	}
	delay_us(104);
	while(((IOPIN0 >> rx) & 1) == 0);
	return rdata;
}
	
int main() {
	init_uart();
	init_lcd();
	init_kpm();
	while(1) {
		uart_tx(keyscan());
		delay_ms(300);
	}
}
