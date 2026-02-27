#include<stdio.h>
struct st 
{
	char a:4;
	char c:1;
}var;
int main()
{
	printf("size of var:%ld\n",sizeof(var));
	return 0;
}
