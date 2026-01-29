#define IOCLR0  (*(volatile unsigned int *)0xE002800C)
#define IODIR0  (*(volatile unsigned int *)0xE0028008) 
#define IOSET0  (*(volatile unsigned int *)0xE0028004)
#define  T0TC  (*(volatile unsigned int *)0xE0004008)
#define T0TCR  (*(volatile unsigned int *)0xE0004004)
#define T0PC  (*(volatile unsigned int *)0xE0004010)
#define T0PR  (*(volatile unsigned int *)0xE000400C)
#define LED  (1<<1)

void delay_sec(unsigned int sec)
{
T0PC=T0TC=0;
T0PR=15000000-1;
T0TCR=1;
while(T0TC<sec);
T0TCR=0;
}

int main()
{
IODIR0=LED;
IOSET0=LED;
while(1)
{
IOCLR0=LED;
delay_sec(1);
IOSET0=LED;
delay_sec(1);
}
return 0;
}


