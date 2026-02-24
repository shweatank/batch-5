#include <stdio.h>
int main() {
	int var1 = 0;
	int res;
	res = var1--;
	printf("%d\n",res);
	var1 = 0;
	res = --var1;
	printf("%d\n",res);
	return 0;
}
