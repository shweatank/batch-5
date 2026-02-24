#include<stdio.h>
 int (sum)(int ,int);
 int (sub)(int ,int);
typedef int (*fptr)(int ,int);
fptr select(void);
char op;
void main()
{
int a,b;
printf("enter values:\n");
scanf("%d%d",&a,&b);
fptr p;
p=select();
int res=p(a,b);
printf("result=%d\n",res);
}
fptr select(void)
{
printf("enter option +,-,/,*\n");
scanf(" %c",&op);
switch(op)
{
case '+':
	return sum;
	break;
case '-':
 return sub;
break; 
}
}
int sum(int a,int b)
{
return a+b;
}
int sub(int a,int b)
{
return a-b;
}

