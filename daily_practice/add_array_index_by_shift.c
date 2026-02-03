#include<stdio.h>
#define MAX 20
void Add(int,int ,int *);
int main()
{
	int n,i,arr[MAX]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
	puts("enter the element");
	scanf("%d",&n);
	puts("enter the index");
	scanf("%d",&i);
	if(i>=MAX || i<0)
		printf("enter the correct index from 0 to %d\n",MAX);
	Add(n,i,arr);
}
void Add(int ele,int index,int *arr)
{
	int i=0;
	int *ptr=arr;
	for(i=MAX-1;i>index;i--)
	{
		arr[i]=arr[i-1];
	}
	arr[index]=ele;
	for(i=0;i<MAX;i++)
	printf("%d  ",ptr[i]);
	printf("\n");
}
