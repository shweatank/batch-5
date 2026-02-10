#include<stdio.h>
void main()
{
	void *vp;
	int a=5;
	float b=9.8;
	char ch='a';

	vp=&a;
	printf("a=%d\n",*(int*)vp);

	vp=&b;
	printf("b=%f\n",*(float*)vp);

	vp=&ch;
	printf("ch=%c\n",*(char*)vp);
}

