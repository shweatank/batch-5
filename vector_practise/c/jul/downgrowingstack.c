#include <stdio.h>
int f1();
int f2();
int f3();
int main() {
	int a = 10,b = 20,c = 30;
	printf("%p\n",&a);
	f1(a);
}

int f1(int num) {
	int local = 10;
	printf("%p\n",&local);
	return 1;
}
