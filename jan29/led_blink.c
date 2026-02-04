#define LED 1
#define IODIR0 (*((volatile unsigned long *) 0xE0028008))
#define IOSET0 (*((volatile unsigned long *) 0xE0028004))
#define IOCLR0 (*((volatile unsigned long *) 0xE002800C))

void delay(int dly)
{
	dly*=12000;
	while(dly--);
}

int main()
{
	IODIR0=1<<LED;
	while(1)
	{
		IOSET0=1<<LED;
		delay(10);
		IOCLR0=1<<LED;
	}
}
