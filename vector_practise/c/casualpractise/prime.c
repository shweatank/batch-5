#include<stdio.h>
void main() {
	int num;
	int count = 0;
	printf("Enter the number: ");
	scanf("%d",&num);
	for(int i=2;i<num/2;i++) {
		if(num%i == 0) {
			count++;
		}
		if(count > 0) { break; }
	}
	if(count == 0) {
		printf("Prime Number\n");
	}
	else {
		printf("Not a prime number\n");
	}
}

