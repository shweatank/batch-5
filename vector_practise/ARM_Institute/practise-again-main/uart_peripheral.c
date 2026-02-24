#include <lpc21xx.h>
#include "lcd.c"

int main() {
	char ch;
	PINSEL0 = 0x05;
	U0LCR = 0x83;
	U0DLM = 0;
	U0DLL = 97;	//	PCLK/(16*baudrate)
	U0LCR = 0x03;
	while(1) {
		while(((U0LSR >> 0) & 1) == 0);
		ch = U0RBR;
		U0THR = ch;
		while(((U0LSR >> 5) & 1) == 0);
	}
}
