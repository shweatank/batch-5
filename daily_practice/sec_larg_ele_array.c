#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int *Input(int );
int SecLarge(int *,int);
void Print(int *,int);
int main()
{
	int n;
	puts("enter the size of array");
	scanf("%d",&n);
	int *a=NULL;
	a=Input(n);
	Print(a,n);
	int num=SecLarge(a,n);
	printf("sec largest num is %d\n",num);
	return 0;
}
int *Input(int n)
{
	int *a=NULL;
	srand(time(0));
	a=malloc(n*sizeof(*a));
	for(int i=0;i<n;i++)
		a[i]=rand()%500;
	return a;
}
void Print(int *a,int n)
{
	int i=0;
	for(;i<n;i++)
		printf("%d ",a[i]);
	puts("");
}
int SecLarge(int *a,int n)
{
	int sec=-1,hig=-1;
	for(int i=0;i<n;i++)
	{
		if(a[i]>hig)
		{
			sec=hig;
			hig=a[i];
		}
		if(a[i]>sec && a[i]<hig)
			sec=a[i];
	}
	return sec;
}
