#include<stdio.h>
int bitswap(int a)
{
    int res=a|(a<<1);

    return res>>1;
}
void binary(int a)
{
	int pos;
	for(pos=31;pos>=0;pos--)
		printf("%d",a>>pos&1);
}

void main()
{
	int a=10;

        binary(a);
	a=bitswap(a);
	printf("\n");
	binary(a);
}
