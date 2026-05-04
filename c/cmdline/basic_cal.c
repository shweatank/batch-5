#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void main(int argc,char ** argv)
{
int n1,n2,n3;
if(argc<4)
{
printf("usage: ./a.out n1 opr n2\n");
return;
}
n1=atoi(argv[1]);
n2=atoi(argv[3]);

switch(argv[2][0])
{
case '+':n3=n1+n2;
printf("n3=%d\n",n3);
break;

case '-':n3=n1-n2;
printf("n3=%d\n",n3);
break;

case '/':n3=n1/n2;
printf("n3=%d\n",n3);
break;

case '*':n3=n1*n2;
printf("n3=%d\n",n3);
break;

}
}

