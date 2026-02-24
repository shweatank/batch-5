#include <stdio.h>
int main() {

	int a = 10;
	int y;
	y = a++; // 10
//	printf("%d\n",y);
	y = 0;
	a = 10;
	y = ++a; // 11
//	printf("%d\n",y);
	float f = 1234.5;
	printf("%e\n %lf\n",f,f);
	return 0;
}

