#include<stdio.h>
int Pow(int );
void Input(int *,int );
void Print(int *,int );
void CheckPow(int *,int );
int main()
{
	int n;
	puts("enter the size");
	scanf("%d",&n);
	int arr[n];
	Input(arr,n);
	Print(arr,n);
	CheckPow(arr,n);
}
void Input(int *arr,int n)
{
	puts("enter the elements");
	for(int i=0;i<n;i++)
	{
		scanf("%d",arr+i);
	}
}
void Print(int *arr,int n)
{
	puts("given elements are");
	for(int i=0;i<n;i++)
	{
		printf("%d ",arr[i]);
	}
	puts("");
}
void CheckPow(int *arr,int n)
{
	puts("pow of 2 elements are");
	for(int i=0;i<n;i++)
	{
		if(Pow(arr[i]))
			printf("%d ",arr[i]);
	}

}
int Pow(int n)
{
	if((n&(n-1))==0)
		return 1;
	else 
		return 0;
}	
