#include<stdio.h>
void main()
{
	int num,pos;
	printf("enter the num\n");
	scanf("%d",&num);
	printf("before\n");
	for(pos=31;pos>=0;pos--)
		printf("%d",num>>pos&1);
	printf("\n");
	printf("enter the pos to set a bit\n");
	scanf("%d",&pos);
	if(pos<=31)
	{
	
	num=num|1<<pos;

	printf("after\n");

	for(pos=31;pos>=0;pos--)
		printf("%d",num>>pos&1);
	printf("\n");
	}
	else
		printf("invalid position\n");
}
