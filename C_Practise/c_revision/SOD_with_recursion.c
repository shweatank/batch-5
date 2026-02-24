#include <stdio.h>

int sumOfDigits(int num) {
	static int sum = 0;
	if(num == 0) return sum;
       sum += num % 10;
       num /= 10;
       sumOfDigits(num);
}

int main() {
	int sumOfDigits = sumOfDigits(12345);
	printf("%d\n",sumOfDigits);
}
