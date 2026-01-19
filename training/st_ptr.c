#include"header.h"
int sum(int,int);
//int sub(int,int);
//int mul(int,int);
//int div1(int,int);
typedef int (*fptr)(int,int);
struct ptr
{
fptr p[3];
};
struct ptr v;
void main()
{
int n1,n2,op;
v.p[0]=sum;
//v.p[1]=sub;
//v.p[2]=mul;
//v.p[3]=div1;
while(1)
{
printf("enter the numbers:\n");
scanf("%d%d",&n1,&n2);
printf("enter options:\n");
printf("1.add 2.sub 3.mul 4.div\n");
scanf("%d",&op);
switch(op)
{
case 1:
printf("%d\n",v.p[0](n1,n2));
break;
}
}
}
int sum(int a,int b)
{
return a+b;
}
