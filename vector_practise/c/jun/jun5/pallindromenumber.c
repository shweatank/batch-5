#include <stdio.h>
#include <stdbool.h>
bool pallindrome(int num);
void main() {
	int num;
	printf("Enter the number: ");
	scanf("%d",&num);
	if(pallindrome(num)) {
		printf("It is a pallindrome number");
	}
	else {
		printf("It is not a pallindrome number");
	}
}

bool pallindrome(int num) {
	int var = num;
	int res = 0;
	while(num != 0) {
		int digit = num % 10;
		res = res*10 + digit;
		num = num / 10;
	}
	return(res == var);
}
