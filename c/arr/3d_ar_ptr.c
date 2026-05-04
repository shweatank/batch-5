#include<stdio.h>
void main()
{
int *p,a[2][2][3];

for(p=&a[0][0][0];p<=&a[1][1][2];p++)
scanf("%d",p);

for(p=&a[0][0][0];p<=&a[1][1][2];p++)
printf("%d",*p);
}

