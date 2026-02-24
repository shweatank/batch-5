#include <stdio.h>
int main() {
static int a = 10;
	char c = 'A';
	double d = 90.118;
	int *pa = &a;
	char *pc = &c;
	double *pd = &d;
	printf("%p\n",pa);
	printf("%p\n",pc);
	printf("%p\n",pd);
	printf("%d\n",a);
	*pa += 10;
	printf("%d\n",*pa);
}

	
