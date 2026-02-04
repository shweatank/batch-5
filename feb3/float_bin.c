#include<stdio.h>

union data
{
	float num;
	int n;
};

int main()
{
	union data d;
	d.num=6.8;
	int i,j=3;
	float num=6.8;
	unsigned char *ptr;
	ptr=(unsigned char*)&num;
	while(j>=0)
	{
		for(i=7;i>=0;i--)
		{
			if((ptr[j]>>i)&1)
				printf("%d ",1);
			else
				printf("%d ",0);
		}
		j--;
	}
	printf("\n");

	for(i=31;i>=0;i--)
	{
		if((d.n>>i)&1)
			printf("%d ",1);
		else
			printf("%d ",0);
	}
	printf("\n");
	return 0;
}
