#include<stdio.h>
#include<stdlib.h>
int cmp_fun(const void *a,const void *b)
{
	return ((*(int *)a) - (*(int*)b));
}
struct abc
{
	int data;
	char name[20];
};
void print_arr(struct abc *ptr,int n)
{
	for(int i=0;i<n;i++)
	{
		printf("roll:%d  name:%s\n",ptr[i].data,ptr[i].name);
	}
}
int main()
{
	struct abc arr[3]={{5,"hi"},{3,"hello"},{4,"namasthe"}};
	qsort(arr,3,24,cmp_fun);
	print_arr(arr,3);
}
