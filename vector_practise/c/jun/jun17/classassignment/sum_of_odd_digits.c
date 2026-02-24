#include <stdio.h>
int main() {
	int num,temp,sum;
	printf("enter the number: ");
	scanf("%d",&num);
	temp = num; sum = 0;
	if(num < 0) temp *= -1;
loop:	if(temp != 0) {
		int curr = temp % 10;
		if(curr % 2 == 1) {
			sum += curr;
		}
		temp /= 10;
		goto loop;
	}
	printf("The sum of odd digits of given number is: %d\n",sum);
}
