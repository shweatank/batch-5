#include"header.h"
int result,n1,n2;
void main()
{
printf("enter two numbers:\n");
scanf("%d%d",&n1,&n2);
if(fork()==0)
{
//result=n1-n2;
result =10;
printf("%d\n",result);
//printf("sub of two numbers %d and %d =%d\n",n1,n2,result);
printf("%p\n",&result);
}
else
{
//result=n1+n2;
//printf("addition of two numbers %d and %d =%d\n",n1,n2,result);
printf("%d\n",result);
printf("%p\n",&result);
}
}
