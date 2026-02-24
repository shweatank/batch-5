#include <stdio.h>
void main() {
	int num,secondmax,temp,max;
	printf("Enter the number: ");
	scanf("%d",&num);
	temp = num;
	if(num < 0) temp *= -1;
	(temp > -10 && temp < 10) ? (max = temp % 10,secondmax = temp % 10) : (max = 0,secondmax = 0);
loop:	if(temp) {
		int curr = temp % 10;
		if(curr > max) {
			secondmax = max;
			max = curr;
		}
		else if(curr != max && curr > secondmax) {
			secondmax = curr;
		}
		temp /= 10;
		goto loop;
	}
	printf("The second largest digit of given number is: %d\n",secondmax);
}
