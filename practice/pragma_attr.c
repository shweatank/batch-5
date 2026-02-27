#include<stdio.h>
//#pragma pack(1)
struct st
{
	int a;
	char c;
	int b;
}__attribute__((packed));
int main()
{
	struct st var;
	printf("size:%ld\n",sizeof(var));
	return 0;
}
