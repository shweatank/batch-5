#include<stdio.h>
void main()
{
int i,j,n,k,t;
printf("enter the arr size:\n");
scanf("%d",&n);
int a[n];
for(i=0;i<n;i++)
scanf("%d",&a[i]);
 i=0;
j=n-1;
while(i<j)
{
	if(a[i]%2==0)
	{
		if(a[j]%2!=0)
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
/*for(i=n-1,k=0;i>=k;i--)
{
if(a[i]==0)
{
t=a[i];
a[i]=a[k];
a[k]=t;
k++;
}
}*/

for(i=0;i<n;i++)
printf("%d ",a[i]);
printf("\n");
}
