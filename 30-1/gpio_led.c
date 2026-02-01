#define LED  (1<<1)
unsigned int *T0TC,T0TC,T0TCR,T0TC;
unsigned int *IOCLR0,*IOSET0;

void delay_sec(unsigned int sec)
{
*T0PC=*T0TC=0;
*T0PR=15000000-1;
*T0TCR=1;
while(*T0TC<sec);
*T0TCR=0;
}

int main()
{



*IODIR0=LED;
*IOSET0=LED;
while(1)
{
*IOCLR0=LED;
delay_sec(1);
*IOSET0=LED;
delay_sec(1);
}
return 0;
}


