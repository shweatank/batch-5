#include "types.h"
void delay_us(u32 delay) {
	delay *= 12;
	while(delay--);
}

void delay_ms(u32 delay) {
	delay *= 12*1000;
	while(delay--);
}

void delay_s(u32 delay) {
	delay *= 12*1000000;
	while(delay--);
}
