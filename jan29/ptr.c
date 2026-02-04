#include<stdio.h>
int main()
{
	void *ptr;
	if(sizeof(ptr)==8)
		printf("32 bit arch\n");
}
