#include<stdio.h>
void main()
{
int i=0,j,k,n,t;
printf("enter the arr size:\n");
scanf("%d",&n);
int a[n];
for(i=0;i<n;i++)
scanf("%d",&a[i]);
j=n-1,i=0;
while(i<j)
{
	if(a[i]==1)
	{
		if(a[j]==0)
		{
			t=a[i];
			a[i]=a[j];
			a[j]=t;
			i++;
			j--;
		}
		else
			j--;
	}
	else
		i++;
}
for(i=0;i<n;i++)
printf("%d ",a[i]);
printf("\n");
}
