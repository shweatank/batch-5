#include<stdio.h>

struct operation
{
	unsigned int op;
	int a;
	int b;
};

int add(int n1,int n2)
{
	return n1+n2;
}

int sub(int n1,int n2)
{
	return n1-n2;
}

int func_ptr_plugin(struct operation *data_ptr)
{
	int (*fptr[2])(int,int)={add,sub};
	return fptr[data_ptr->op](data_ptr->a,data_ptr->b);
}

int main()
{
	struct operation data={0,10,20};
	printf("result :%d\n",func_ptr_plugin(&data));
}
