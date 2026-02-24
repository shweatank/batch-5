#include <stdio.h>
int sum(int num) {
	if(!num) return 0;
	int r = num % 10;
	return r+sum(num/10);
}
int main() {
	int num;
	printf("Enter the number:");
	scanf("%d",&num);
	printf("The sum of digits is: %d\n",sum(num));
}
