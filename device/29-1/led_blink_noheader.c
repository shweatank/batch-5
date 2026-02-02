#define IOPIN0 *((volatile unsigned int *)0Xe0028000)
#define IOSET0 *((volatile unsigned int *)0Xe0028004)
#define IODIR0 *((volatile unsigned int *)0Xe0028008)
#define IOCLR0 *((volatile unsigned int *)0Xe002800c)
#include<stdio.h>

void delayus(unsigned int delay)
{
	delay*=12;
	while(delay--);
}


int main()
{
	IODIR0|=1<<7;
	IOSET0=1<<7;
	delayus(100);
	IOCLR0=1<<7;
	delayus(100);
//	unsigned int IOPIN=*((volatile unsigned int *)0xe0028000);
//	printf("%u",IOPIN);

}

