#include<stdio.h>
void main()
{
short int num;
printf("enter the num\n");
scanf("%hd",&num);
int pos;
for(pos=15;pos>=0;pos--)
printf("%hd ",num>>pos&1);
printf("\n");
num=num|1<<15;
for(pos=15;pos>=0;pos--)
printf("%hd ",num>>pos&1);
}
