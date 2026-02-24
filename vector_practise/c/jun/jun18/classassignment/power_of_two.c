#include <stdio.h>
int main() {
	int num;
	printf("Enter the number: ");
	scanf("%d",&num);
	while(num % 2 == 0 && num != 1) {
		num /= 2;
	}
	(num == 1) ? printf("Power of 2\n") : printf("Not a power of 2\n");
}

