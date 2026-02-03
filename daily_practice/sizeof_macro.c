#include<stdio.h>
#define MY_SIZE(x) ((char *)(&x+1)-(char *)(&x))
int main()
{
	int x;
	float y;
	double z;
	char c;
	printf("%ld\n",MY_SIZE(x));
	printf("%ld\n",MY_SIZE(y));
	printf("%ld\n",MY_SIZE(z));
	printf("%ld\n",MY_SIZE(c));
}
