#include<stdio.h>
int main()
{
	int n,t;
	scanf("%d",&n);t=n;
	n|=((n&0x80)>>7)|((n&0x01)<<7);
	printf("num:%d\n",n);
	n=t;
	n|=((n&0x80000000)>>31)|((n&0x01)<<31);
	printf("num:%d\n",n);
}
