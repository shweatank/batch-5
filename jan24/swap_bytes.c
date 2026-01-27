#include<stdio.h>
int main()
{
	unsigned int num=0x12345678;
	printf("num : %x\n",num);
	num=(((num&0x0000ff00)<<8) | ((num&0x000000ff)<<24) | ((num&0x00ff0000)>>8) | ((num&0xff000000)>>24));
	printf("swapped num : %x\n",num);
}
