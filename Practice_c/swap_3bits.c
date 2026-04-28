#include<stdio.h>

int main()
{
	int x=0xcb;
	x=((~(7<<1)  & ~(7<<5) & x)|(((x>>1)&7)<<5)|(((x>>5)&7)<<1));
	printf("%x\n",x);
}
