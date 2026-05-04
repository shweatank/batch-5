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
printf("enter the pos\n");
scanf("%d",&pos);
num=num&~(1<<pos);
for(pos=31;pos>=0;pos--)
printf("%d ",num>>pos&1);
printf("\n");
}
