#include<stdio.h>
void main()
{
int num;
printf("enter the num\n");
scanf("%d",&num);
int pos;
for(pos=31;pos>=0;pos--)
printf("%d ",num>>pos&1);
printf("\n");

for(pos=31;pos>=0;pos--)
{
int r=num&1;
if(r)
num=num>>1;
num=num&~(1<<pos);
}

for(pos=31;pos>=0;pos--)
printf("%d ",num>>pos&1);
}
