#include<stdio.h>

int check_kth_bit(int num,int bitpos)
{
	return (num>>bitpos)&1;
}

int main()
{
	int num=8;
	printf("number : %d\n",num);
	if(check_kth_bit(num,3))
		printf("bit is set\n");
	else
		printf("bit is not set\n");
}
