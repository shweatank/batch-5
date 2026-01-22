#include<stdio.h>
void Input(int *,int);
double Sum(int *,int);
void Avg(int,double);
int main()
{
        int n;
        puts("enter the size");
        scanf("%d",&n);
        int arr[n];
        Input(arr,n);
        double sum=Sum(arr,n);
        Avg(n,sum);
}
void Input(int *arr,int s)
{
        puts("enter the elements");
        for(int i=0;i<s;i++)
                scanf("%d",arr+i);
}
double Sum(int *arr,int s)
{
        double sum=0;
        for(int i=0;i<s;i++)
                sum+=arr[i];
        return sum;
}
void Avg(int n,double sum)
{
        float avg=sum/n;
        printf("average is %.2f\n",avg);
}
