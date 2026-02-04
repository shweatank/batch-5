//2> insert an element in an array at specific index by using right shift of array that shifting done by using pointers 
#include<string.h>
#include<stdio.h>
int SIZE=5;

void init_array(int *arr)
{
	for(int i=0;i<SIZE;i++)
	{
		printf("enter elements : ");
		scanf("%d",&arr[i]);
	}
}

void print_array(int *arr)
{
	for(int i=0;i<SIZE;i++)
	{
		printf("%d ",arr[i]);
	}
	printf("\n");
}

int  insert_array(int *arr,int pos,int val)
{
	if(pos>(SIZE-1))
		return 0;
	memmove(arr+pos+4,arr+pos,sizeof(int)*(SIZE-pos-1));
	arr[pos]=val;	
}

int main()
{
	int arr[SIZE],pos,val;
	init_array(arr); print_array(arr);
	printf("enter pos and val : ");
	scanf("%d%d",&pos,&val);
	insert_array(arr,pos,val);
	print_array(arr);
}
