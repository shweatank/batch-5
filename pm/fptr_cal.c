#include"header.h"
int sum(int,int);
typedef int(*fptr)(int,int);
fptr p;
void main()
{
int n1,n2;
char op;
printf("enter the two values:");
scanf("%d%d",&n1,&n2);
printf("\033[33mmenter the options\n");
printf("1.+\t2.-\t3.*\t4./\t5.%%\033[0m\n");
scanf(" %c",&op);
switch(op)
{
case '+':p=sum;
printf("sum=%d\n",p(n1,n2));
break;
}
}
int sum(int a,int b)
{
return a+b;
}
