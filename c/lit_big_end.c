#include<stdio.h>
void main()
{
int x=0x1;
if((char)x&1)
printf("Little endian\n");
else
printf("Big endian\n");
}
