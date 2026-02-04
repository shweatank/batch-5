#include<stdio.h>

void swap_bytes(int *n)
{
	int i;
	unsigned char *num=(char*)n;
	for(i=0;i<4;i++)
	{
		num[i]=((num[i]<<4)|(num[i]>>4));
	}
}

int main()
{
	int num=0x12345678;
	printf("number : %x\n",num);
	swap_bytes(&num);
	printf("after swapping : %x\n",num);
}
