/* inserting element in the given index*/

#include<stdio.h>
void main(){
	int a[6],ele=5,i;
	printf("Enter 5 elements\n");
	for(i=0;i<5;i++)
		scanf("%d",&a[i]);
	printf("BEFORE:\n");
	for(i=0;i<5;i++)
		printf("%d ",a[i]);

	int index,num;
	printf("\nEnter the number and index to insert\n");
	scanf("%d%d",&index,&num);
/*	if(index>(ele))
		index=ele;*/

	for(i=5;i>=index;i--)
		a[i]=a[i-1];
	a[index]=num;
	printf("After:\n");
	for(i=0;i<6;i++)
		printf("%d ",a[i]);
printf("\n");
}
