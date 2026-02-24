#include <stdio.h>
int main() {
	int num,temp,maxdigit;
	printf("Enter the number: ");
	scanf("%d",&num);
	temp = num;
	maxdigit = 0;
loop:	if(temp != 0) {
		int current = temp % 10;
		if(current > maxdigit) maxdigit = current;
		temp /= 10;
		goto loop;
	}
	printf("The maximum digit of given number is: %d\n",maxdigit);
}
