#include <stdio.h>
void main() {
	int num,temp,sum;
	printf("Enter the number:");
	scanf("%d",&num);
	temp = num ; sum = 0;
	if(num < 0) temp *= -1;
	while(temp) {
		if((temp % 10) % 2 == 0) sum += temp % 10;
		temp /= 10;
	}
	printf("the sum of even digits of the given number is: %d\n",sum);
}
