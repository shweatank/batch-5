#include<stdio.h>
#define leds 0
volatile unsigned int *SEL0=(unsigned int *)0xE002C000;
volatile unsigned int *IODIR0=(unsigned int *)0xE0028008 ;
volatile unsigned int *IOPIN0=(unsigned int *)0XE0028000;
/*#define SEL0 0xE002C000
#define IOPIN0 0XE0028000
#define IODIR0 0XE0028008*/
int main(){
	int i;
	while(1){
	*SEL0=(*SEL0&~(0x03));
	*IODIR0|=0X0F;
	*IOPIN0=1<<leds;
	for(i=0;i<1000000;i++);
	*IOPIN0=0<<leds;
	}
}
