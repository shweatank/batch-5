#include<stdio.h>
void main()
{
unsigned int num=0x18283848;
printf("before=%x\n",num);
num=(num&0x0f000000)<<4|(num&0xf0000000)>>4 | (num&0x00f00000)>>4|(num&0x000f0000)<<4|(num&0x0000f000)>>4|(num&0x00000f00)<<4|(num&0x000000f0)>>4|(num&0x0000000f)<<4;
/*int r1=(num&0xf0f0f0f0)>>4;
int r2=(num&0x0f0f0f0f)<<4;
num=r1|r2;*/
printf("after=%x\n",num);
}
