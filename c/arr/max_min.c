#include<stdio.h>
void main()
{
int a[5],max,min,i,ele;
ele=sizeof(a)/sizeof(a[0]);
printf("enter arr ele\n");
for(i=0;i<ele;i++)
scanf("%d",&a[i]);

max=a[0];
min=a[0];
for(i=1;i<ele;i++)
{
if(a[i]>max)
max=a[i];
else if(a[i]<min)
min=a[i];

}

printf("max=%d min=%d\n",max,min);
}
