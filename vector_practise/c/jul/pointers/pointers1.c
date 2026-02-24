#include <stdio.h>
int main() {
	int a = 258,b=9;
	unsigned char *pa = &a;
	unsigned char *demo = (pa+10);

	printf("The value accessed by pointer pa: %d\n",*pa);
	printf("The address of pa: %p\n",pa);

	printf("The starting address of a: %p\n",&a);
//	printf("The ending address of a: %p\n",(&a)-1);

//	printf("The starting address of b: %p\n",&b);

	printf("The address of pa++: %p\n",demo);
	printf("The value accessed by demo pointer: %d\n",*demo);
}

