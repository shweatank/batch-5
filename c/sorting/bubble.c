#include<stdio.h>

void bubble_sort(int *,int);
void print_array(int *,int);

void main()
{
	int a[5],i,ele;
	ele=sizeof(a)/sizeof(a[0]);
	printf("enter array eles\n");

	for(i=0;i<ele;i++)
		scanf("%d",&a[i]);
	printf("before sorting\n");
	print_array(a,ele);
	bubble_sort(a,ele);
	printf("\nafter sorting\n");
	print_array(a,ele);

}
void bubble_sort(int * a,int ele)
{
	int i,j;
	for(i=0;i<ele-1;i++)
		for(j=0;j<ele-i-1;j++)
			if(a[j]>a[j+1])
			{
				int temp=a[j];
				a[j]=a[j+1];
				a[j+1]=temp;
			}
}

void print_array (int* a,int ele)
{
	int i;
	for(i=0;i<ele;i++)
		printf("%d ",a[i]);
}
