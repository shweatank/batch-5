#include<stdio.h>
void main()
{
unsigned int num;
int pos,r;
printf("enter the num\n");
scanf("%d",&num);

for(pos=31;pos>=0;pos--)
printf("%d ",num>>pos&1);
printf("\n");

for(pos=31;pos>=0;pos--)
{
if(pos%2==0)
num=num|1<<pos;
else
num=num&~(1<<pos);
}

for(pos=31;pos>=0;pos--)
printf("%d ",num>>pos&1);
printf("\n");
}


