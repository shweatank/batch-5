/*input: {1 2 3 4 5}
output:  {4 5 1 2 3}
 */

#include<stdio.h>
void main(){

	int a[5],i;
	printf("Enter 5 elements\n");
	for(i=0;i<5;i++)
		scanf("%d",&a[i]);

	printf("Before:\t");
	for(i=0;i<5;i++)
		printf("%d ",a[i]);

	int rotate;
	printf("Enter no.of rotations\n");
	scanf("%d",&rotate);
	int temp;
	for(i=0;i<rotate;i++){
		temp=a[4];
		for(int j=5-1;j>=0;j--)
			a[j]=a[j-1];
		a[0]=temp;
	}
	printf("After:\t");
	for(i=0;i<5;i++)
		printf("%d ",a[i]);
}
