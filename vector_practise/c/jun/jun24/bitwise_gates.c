#include <stdio.h>
int main() {
	int a = 100;
	printf("%d\n",a&0); // always zero
	printf("%d\n",a&-1); // always the other operand(eg: a)
	printf("%d\n",a|0); // always the other operand(eg: a)
	printf("%d\n",a|-1);// always -1
	printf("%d\n",0^a);// always other operand(eg: a)
	printf("%d\n",-1^a);// always 1's complement of other operand
	return 0;
}
