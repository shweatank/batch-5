#include<stdio.h>
void main()
{
int num=0x11223344;
printf("before =%x\n",num);
num=(num&0xf0000000)>>28|(num&0x0ffffff0)|(num&0x0000000f)<<28;
printf("before =%x\n",num);
}

