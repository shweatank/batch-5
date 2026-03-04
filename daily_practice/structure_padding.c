#include<stdio.h>
//#pragma pack(4)
struct st
{
	int a:1;
	char ch:1;
};
int main()
{
	struct st *c;
	scanf("%d",&c->a);
	printf("%ld\n",sizeof(c));

}
