#include<stdio.h>
void main()
{
	int a[10],i;
	int even=0,odd=0;
	for(i=0;i<10;i++)
	{
		scanf("%d",&a[i]);
	}
	for (i=0;i<10;i++)
	{
		if (a[i]%2==0)
			even=even++;
		else
			odd=odd++;
	}
	printf("\n even elements are=%d",even);
	printf("\n odd elements are=%d",odd);
}

