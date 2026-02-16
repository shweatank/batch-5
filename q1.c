/*
 * Input (in_buf) (12 bytes):

struct {
  uint32_t op;     // 1=op1, 2=op2
  int32_t  a;
  int32_t  b;
};


Output (out_buf) (4 bytes):

int32_t result;


Constraints: Implementation must use a struct of function pointers for dispatch*/
#include<stdio.h>
#include<stdlib.h>
typedef unsigned int uint32_t;
struct st{
  uint32_t op;     // 1=op1, 2=op2
  int32_t  a;
  int32_t  b;
};
struct fptr
{
	int32_t (*fp)(int32_t,int32_t);
};
int32_t add(int32_t x,int32_t y){return x+y;}
int32_t sub(int32_t x,int32_t y){return x-y;}
int32_t operation(struct fptr var,int32_t m,int32_t n)
{
	int32_t res=var.fp(m,n);
	return res;
}
int main()
{
	int32_t out;
	struct st in;
	struct fptr arr[2]={add,sub};
	scanf("%u %d%d",&in.op,&in.a,&in.b);
	out=operation(arr[in.op],in.a,in.b);
	printf("%d\n",out);
	return 0;
}
