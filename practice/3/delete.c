#include<stdio.h>
void main(){

int a[5],i;
printf("Enter 5 elements\n");
for(i=0;i<5;i++)
scanf("%d",&a[i]);
printf("Before:\t");
for(i=0;i<5;i++)
printf("%d ",a[i]);
printf("\n");
int index;
printf("Enter the index to delete\n");
scanf("%d",&index);
if(index>=5)
{
printf("Invalid index\n");
return;
}
for(i=index;i<5-1;i++)
a[i]=a[i+1];
printf("After:\t");
for(i=0;i<4;i++)
printf("%d ",a[i]);
}
