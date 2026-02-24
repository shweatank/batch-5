#include <stdio.h>
#include <stdlib.h>
//int printf(const char*,...);
void f1();
int f2();
void f0() 
{
	f1();
	printf("before the main block\n");
	exit(0);
}

void f1() {
	f2();
	printf("In the main block\n");
}
int f2() {
	printf("After the main block\n");
	return 0;
}
