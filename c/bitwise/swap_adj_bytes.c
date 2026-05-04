#include<stdio.h>
void main()
{
unsigned int num=0x11223344;
printf("num=%x\n",num);
num=(num&0xff000000)>>8|(num&0x00ff0000)<<8|(num&0x0000ff00)>>8|(num&0x000000ff)<<8;
printf("%x\n",num);
}
