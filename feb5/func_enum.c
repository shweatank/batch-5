#include<stdio.h>

int add(int x,int y)
{
	return x+y;
}

int sub(int x,int y)
{
	return x-y;
}

int main()
{
	int(*fptr[])(int,int)={add,sub};
	enum c{ADD,SUB};
	int choice;
	printf("enter choice : 0.add 1.sub\n");
	scanf("%d",&choice);
	if(choice==ADD)
		printf("result  : %d\n",fptr[0](10,20));
	else if(choice==SUB)
		printf("result  : %d\n",fptr[1](10,20));
	else
		printf("invalid choice\n");
	return 0;
}
