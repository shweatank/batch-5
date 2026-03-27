#define PINSEL0  (*(volatile unsigned int*)0xE002C000)
#define IO0DIR   (*(volatile unsigned int*)0xE0028008)
#define IO0SET   (*(volatile unsigned long *)0xE0028004)
#define IO0CLR   (*(volatile unsigned long *)0xE002800C)


#define T0IR   (*(volatile unsigned long *)0xE0004000)
#define T0TCR  (*(volatile unsigned long *)0xE0004004)
#define T0TC   (*(volatile unsigned long *)0xE0004008)
#define T0PR   (*(volatile unsigned long *)0xE000400C)

void dela_ms(unsigned int ms)
{

	T0TCR=0X02;
	T0TC=0;
	T0PR=15000-1;
	T0TCR=1;

	while(T0TC<ms);

	T0TCR=0;
}

int main()
{
        
	  PINSEL0 &=~(0x03);

	  IO0DIR |=1<<0;


	  while(1)
	  {
		  IO0SET|=1<<0;

		  delay_ms(200);

		  IO0CLR|=1<<0;

		  delay_ms(200);
	  }
}

