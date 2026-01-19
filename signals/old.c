#include"header.h"
signalhandler_t p;
void ISR2(int n)
{
printf("SIGQUIT recieved\n");
}
void ISR1(int n)
{
printf("SIGINT recieved\n");
}
void main()
{
siganl(SIGINT,ISR1);
p=signal(SIGQUIT,ISR2);
p(SIGINT);
}
