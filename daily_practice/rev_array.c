#include<stdio.h>
void Input(int *,int);
void Print(int *,int);
void Rev(int *,int);
int main()
{
        int n;
        puts("enter the size");
        scanf("%d",&n);
        int arr[n];
        Input(arr,n);
        Print(arr,n);
        Rev(arr,n);
        Print(arr,n);
}
void Input(int  *arr,int s)
{
        puts("enter the elements");
        for(int i=0;i<s;i++)
                scanf("%d",arr+i);
}
void Print(int *arr,int s)
{
        puts("elements are ");
        for(int i=0;i<s;i++)
                printf("%d ",arr[i]);
        printf("\n");
}
void Rev(int *arr,int s)
{
        int i,j,temp;
        for(i=0,j=s-1;i<j;i++,j--)
        {
                temp=arr[i];
                arr[i]=arr[j];
                arr[j]=temp;

        }
}
