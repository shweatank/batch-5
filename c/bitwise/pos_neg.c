#include<stdio.h>
void main()
{
int num;
printf("enter the num\n");
scanf("%d",&num);
if(num&1<<31)
printf("neg\n");
else
printf("pos\n");
}
