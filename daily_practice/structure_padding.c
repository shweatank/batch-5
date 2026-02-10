#include<stdio.h>
//#pragma pack(4)
struct st
{
	int a:31;
	char ch:1;
};
int main()
{
	struct st c;
	printf("%ld\n",sizeof(c));

}
