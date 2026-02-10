#include<stdio.h>
void main()
{
	int marks[5],i;
	int sum=0;
	float avg;
	for(i=0;i<5;i++)
	{
		scanf("%d",&marks[i]);
	}
	for(i=0;i<5;i++)
	{
		sum=sum+marks[i];
		printf("%d",sum);
	}
	avg=sum/5;
	printf("%f",avg);
}

