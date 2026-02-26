#define T0PC (*(volatile unsigned long *)0xE0004010)
#define T0PR (*(volatile unsigned long *)0xE000400C)
#define T0TC (*(volatile unsigned long *)0xE0004008)
#define T0TCR (*(volatile unsigned long *)0xE0004004)
#define IODIR0 (*(volatile unsigned long *)0xE0028008)
#define IOSET0 (*(volatile unsigned long *)0xE0028004)
#define IOCLR0 (*(volatile unsigned long *)0xE002800C)
void delay_ms(unsigned int ms)
{
	T0PC=0;
	T0PR=15000-1;
	T0TC=0;
	T0TCR=1;
	while(T0TC<ms);
	T0TCR=0;
}
int main()
{
	IODIR0=0XFF;
	IOSET0=0XFF;
	delay_ms(100);
	for(int i=0;i<8;i++)
	{
		IOCLR0=1<<i;
		delay_ms(100);
		IOSET0=1<<i;
		delay_ms(100);
	}
}

