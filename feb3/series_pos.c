#include<stdio.h>

int pos;

int check_2set_bits(int num)
{
	int i=0,cnt=0;
	for(i=31;i>=0;i--)
	{
		if((num>>i)&1)
			cnt++;
	}
	if(cnt==2)
		return 1;
	return 0;
}

int main()
{
	int cur=0;
	printf("enter pos : ");
	scanf("%d",&pos);
	for(int i=0;i<100000;i++)
	{
		if(check_2set_bits(i))
		{
			cur++;
			if(cur==pos)
			{
				printf("num : %d\n",i);
				return 0;
			}
		}
	}
}
