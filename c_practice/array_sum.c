#include<stdio.h>

void init_array(int arr[],int s)
{
        int i;
        for(i=0;i<s;i++)
        {
                printf("enter arr[%d] : ",i);
                scanf("%d",&arr[i]);
        }
}

void print_array(int arr[],int s)
{
        int i;
        for(i=0;i<s;i++)
        {
                printf("arr[%d] : %d\n",i,arr[i]);
        }
}

void array_sum(int arr[],int s)
{
        int sum=0;
        for(int i=0;i<s;i++)
                sum+=arr[i];
        printf("array sum : %d\n",sum);
}
int main()
{
        int arr[50];
        int size;
        printf("enter array size : ");
        scanf("%d",&size);
        init_array(arr,size);
        print_array(arr,size);
        array_sum(arr,size);
}
