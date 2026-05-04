//if signal 2 comes for 4th time restore default value
//if signal 3 comes for 3rd time do the same

#include<stdio.h>
#include<signal.h>
#include<unistd.h>
static int c=0,c1=0;
void isr (int n)
{
if(n==2)
{
c++;
if(c==4)
signal(2,SIG_DFL);
}
if(n==2)
{
c1++;
if(c1==3)
signal(3,SIG_DFL);
}
}
void main()
{
signal(2,isr);
signal(3,isr);
while(1);
}
