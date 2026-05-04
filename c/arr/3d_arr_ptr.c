#include<stdio.h>
void main()
{
int *p;
int a[2][3][3];
printf("enter arr eles\n");
for(p=&a[0][0][0]; p<=&a[1][2][2];p++)
scanf("%d",p);

for(p=&a[0][0][0]; p<=&a[1][2][2]; p++)
printf("%d ",*p);
}
