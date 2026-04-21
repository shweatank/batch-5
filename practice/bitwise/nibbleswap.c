#include<stdio.h>
void main()
{
	int r1,r2,res;
	int num=0x765432;
	r1=(num&0xf0f0f0)>>4;
	r2=(num&0x0f0f0f)<<4;
	res=r1|r2;
	printf("%x",res);
}
