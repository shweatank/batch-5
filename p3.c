#include<stdio.h>
void main()
{
	int num,pos;
	printf("enter the number\n");
	scanf("%d",&num);

	for(pos=31;pos>=0; pos--)
		printf("%d ",num>>pos&1);
	printf("\n");
	printf("enter the pos to toggle\n");
	scanf("%d",&pos);
	num=num^1<<pos;

	for(pos=31;pos>=0; pos--)
		printf("%d ",num>>pos&1);
	printf("\n");
	if((num>>0&1)==(num>>2&1))
		printf("bit 0 and bit 2 are equal\n");
	else
		printf("not equal\n");

}
