#include<stdio.h>
int sum(int,int);
typedef int (*fptr)(int,int);
fptr p;
void main()
	{
int a=10,b=20;
p=sum;
p(a,b);
printf("%d\n",p(a,b));
}
int sum(int a,int b)
{
return a+b;
}
