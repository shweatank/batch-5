#include<stdio.h>
void main()
{
unsigned int n=0xabcd;
printf("%x\n",n);

//n=(n&0xf000)|(n&0x0f00)>>4|(n&0x00f0)<<4|(n&0x000f);
n=(n&0xf00f)|(n&0x0f00)>>4|(n&0x00f0)<<4;
printf("%x\n",n);
}
