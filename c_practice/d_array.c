#include<stdio.h>
#include<stdlib.h>

void print_array(int *arr,int size)
{
        for(int i=0;i<size;i++)
        {
                printf("arr[%d] : %d\n",i,arr[i]);
        }
}

int main()
{
        int *arr=NULL,size;
        printf("enter array size : ");
        scanf("%d",&size);
        arr=realloc(arr,(size+1)*sizeof(*arr));
        for(int i=0;i<size;i++)
        {
                printf("enter arr[%d] : ",i);
                scanf("%d",&arr[i]);
        }
        print_array(arr,size);
}
