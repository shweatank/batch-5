#include<stdio.h>
int main()
{
   unsigned int num=0x44332211;
   int r,r1,r2,r3;
   r=(num>>24)&0x000000ff;
   r1=(num<<24)&0xff000000;
   r2=(num>>8)&0x0000ff00;
   r3=(num<<8)&0x00ff0000;
   num=r|r1|r2|r3;
   printf("%x\n",num);
} 
         
