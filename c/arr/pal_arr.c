#include<stdio.h>
void main()
{
int a[5],t[5],i,j,ele;
ele=sizeof(a)/sizeof(a[0]);
printf("enter arr eles\n");
for(i=0;i<ele;i++)
scanf("%d",&a[i]);

for(i=0;i<ele;i++)
t[i]=a[i];

for(i=0,j=ele-1;i<j;i++,j--)
{
int temp=a[i];
a[i]=a[j];
a[j]=temp;
}
for(i=0;i<ele;i++)
printf("%d ",a[i]);
printf("\n");

for(i=0;i<ele;i++)
{
if(t[i]!=a[i])
{
printf("not pal\n");
break;
}
}

}

