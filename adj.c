#include<stdio.h>
void main()
{
unsigned int num=0x12345678;
num=(num&0xf0000000)>>4|(num&0x0f000000)<<4|(num&0x00f00000)>>4|(num&0x000f0000)<<4|(num&0x0000f000)>>4|(num&0x00000f00)<<4|(num&0x000000f0)>>4|(num&0x0000000f)<<4;
printf("%x\n",num);
}

