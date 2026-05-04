#include<stdio.h>
void main()
{
unsigned int num;
int i,j,pos;

printf("enter the num\n");
scanf("%d",&num);

num=((num>>1)&0x55555555)|((num&0x55555555)<<1);
num=((num>>2)&0x33333333)|((num&0x33333333)<<2);
num=((num>>4)&0x0f0f0f0f)|((num&0x0f0f0f0f)<<4);
num=((num>>8)&0x00ff00ff)|((num&0x00ff00ff)<<8);
num=((num>>16)|(num<<16));

for(pos=31;pos>=0;pos--)
printf("%d ",num>>pos&1);
}
