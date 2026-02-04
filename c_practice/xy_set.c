#include<stdio.h>
int cnt;
int main()
{
	int i,n1=3,n2=8;
	for(i=0;i<32;i++)
	{
		if(((n1>>i)&1)!=((n2>>i)&1))
			cnt++;
	}
	printf("count : %d \n",cnt);
}
