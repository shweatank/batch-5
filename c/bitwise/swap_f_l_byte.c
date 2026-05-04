#include<stdio.h>
void main()
{
int num=0x11223344;
printf("before = %x\n",num);
num=(num&0x00ffff00)|(num&0xff000000)>>24|(num&0x000000ff)<<24;
printf("after = %x\n",num);
}

