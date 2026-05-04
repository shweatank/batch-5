#include<stdio.h>
void main()
{
unsigned int num=0x1234;
//printf("enter num\n");
//scanf("%x",&num);
num=(num&0x0ff0)>>4;
printf("%x\n",num);
}
