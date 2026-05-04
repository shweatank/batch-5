#include<stdio.h>
#include<signal.h>
#include<unistd.h>
void isr(int n)
{
signal(2,SIG_DFL);
signal(2,SIG_DFL);
}
void main()
{
signal(2,SIG_IGN);
signal(3,SIG_IGN);
alarm(10);
while(1);
}

