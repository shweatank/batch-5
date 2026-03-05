#include<stdio.h>
void main()
{
	int num,pos;
	printf("enter the num\n");
	scanf("%d",&num);
	
	for(pos=31; pos>=0; pos--)
		printf("%d", num>>pos&1);

	printf("enter the pos to clear the bit\n");
	scanf("%d",&pos);
	if(pos<=31)
	num=num & ~(1<<pos);
	else
		printf("invalid pos\n");
for(pos=31; pos>=0; pos--)
	printf("%d", num>>pos&1);
}
