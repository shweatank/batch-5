#include<stdio.h>
void Input(int *,int);
void Sum(int *,int);
int main()
{
        int n;
        puts("enter the size");
        scanf("%d",&n);
        int arr[n];
        Input(arr,n);
        Sum(arr,n);
}
void Input(int *arr,int s)
{
        puts("enter the elements");
        for(int i=0;i<s;i++)
                scanf("%d",arr+i);
}
void Sum(int *arr,int s)
{
        int sum=0;
        for(int i=0;i<s;i++)
                sum+=arr[i];
        printf("sum of elements %d",sum);
}
