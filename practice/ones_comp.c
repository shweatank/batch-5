#include<stdio.h>
int main()
{
	int n,bin[32]={0},ones[32]={0},i=0,carry=0,c=0;
	scanf("%d",&n);
	c=~n;
	printf("c=%d\n",c);

	for(int k=31;k>=0;k--)
	{
		printf("%d ",(c>>k)&1);
	}
	printf("\n");
	while(n)
	{
		bin[i++]=n%2;
		n/=2;
	}
	i--;
	printf("binary:");
	for(int k=31;k>=0;k--)
	{
		printf("%d ",bin[k]);
	}
	printf("\n");
	for(int j=0;j<=31;j++)
	{
		ones[j]=(bin[j]==0)?1:0;
	}
	printf("ones comp:");
	for(int k=31;k>=0;k--)
	{
		printf("%d ",ones[k]);
	}
	printf("\n");
	if(ones[0]==0)
		ones[0]=1;
	else
	{
		carry=1;
		ones[0]=0;
		for(int k=1;k<=31;k++)
		{
			if(ones[k]==0)
			{
				ones[k]=1;
				break;
			}	
			else
			{
				ones[k]=0;
				carry=1;
			}	
		
		}
	}
	printf("2's comp:");
	for(int k=31;k>=0;k--)
	{
		printf("%d ",ones[k]);
	}
	printf("\n");

}
