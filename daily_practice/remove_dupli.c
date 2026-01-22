#include<stdio.h>
#include<string.h>
int Remove(int *,int );
void Input(int *,int );
void Print(int *,int );
int main()
{
	int n;
	puts("enter the size");
	scanf("%d",&n);
	int arr[n];
	Input(arr,n);
	Print(arr,n);
	n=Remove(arr,n);
	Print(arr,n);
	
}
void Input(int *arr,int n)
{
	puts("enter the elements");
	int i=0;
	for(;i<n;i++)
		scanf("%d",arr+i);
}
void Print(int *arr,int n)
{
	int i=0;
	for(;i<n;i++)
		printf("%d ",arr[i]);
	printf("\n");
}
int Remove(int *arr,int n)
{
	int i=0;
       for(;i<n;i++)
       {
	       for(int j=i+1;j<n;j++)
	       {
			if(arr[i]==arr[j])
			{
				memmove(arr+j,arr+j+1,(n-j)*sizeof(int));
				j--;
				n--;
			}

	       }
       }
       return n;
}
