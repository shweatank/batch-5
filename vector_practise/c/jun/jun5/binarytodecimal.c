#include <stdio.h>
int decimal(int num) {
	int res = 0;
	int multiplier = 1;
	while(num > 0) {
		int digit = num % 10;
		res = res + digit*multiplier;
		multiplier *= 2;
		num = num/10;
	}
	return res;
}

void main() {
	int num;
	printf("Enter the binary number: ");
	scanf("%d",&num);
	int res = decimal(num);
	printf("The decimal equivalent is: %d\n",res);
}
		

