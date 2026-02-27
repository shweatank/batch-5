#include<stdio.h>
#define FIND_ENDIAN ((*((unsigned char *)&((unsigned int){1})))==1)
#define SIZEOF(x) (((unsigned char *)(&x+1))-((unsigned char *)&x))
int main()
{
	if(FIND_ENDIAN)
	{
		printf("little endian\n");
	}
	else
		printf("big endian\n");

	struct dum
	{
		int s;int h;
	}y;
	printf("size of y is:%ld\n",SIZEOF(y));

}
