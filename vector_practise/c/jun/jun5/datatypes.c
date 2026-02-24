#include <stdio.h>
int main() {
	char var1;
	char var2 = 'A';
	putchar(var1);	
	putchar(var2);
	putchar(65);
	putchar(97);
	putchar(53);
	var1 = var2 + 32;
	var2 = var2 - 32;
	putchar(var1);
	putchar(var2);
	puts("Hello World");
	return 0;
}
