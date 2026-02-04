#include<stdio.h>
int main()
{
	int i,num=126;
	int cnt=0;
	for(i=0;i<32;i++)
	{
		if((num>>i)&1)
		{
			cnt++;
		}
	}

	printf("set bit cnt : %d\n",cnt);
}
