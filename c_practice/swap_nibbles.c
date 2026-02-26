#include<stdio.h>
int main()
{
	unsigned short int num=0x1234;
	printf("num : %x\n",num);
	num=(((num&0x000f)<<4) | ((num&0x00f0)>>4) | ((num&0xf000)>>4) | ((num&0x0f00)<<4));
	printf("num : %x\n",num);
}
