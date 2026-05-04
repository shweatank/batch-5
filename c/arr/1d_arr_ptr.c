#include<stdio.h>
void main()
{
int *p;
int a[5];

for(p=&a[0];p<=&a[4];p++)
scanf("%d",p);

for(p=&a[0];p<=&a[4];p++)
printf("%d",*p);
}

