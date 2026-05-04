#include<stdio.h>
#include<signal.h>
void isr(int num)
{
signal(2,SIG_DFL);
printf("sig 2 handled\n");
}
void main()
{
signal(2,isr);
while(1);
}
