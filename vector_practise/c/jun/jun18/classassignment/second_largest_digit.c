#include <stdio.h>
void main() {
	int num,temp,max,secondmax;
	printf("Enter the number: ");
	scanf("%d",&num);
	temp = num;
	if(temp < 0) temp *= -1;
	(temp < 10) ? (max = secondmax = temp) : (max = secondmax = 0);
	max= secondmax = 0;
//	printf("%d",temp %10);
	while(temp) {
		int curr = temp % 10;
		if(curr > max) {
			secondmax = max;
			max = curr;
		}
		if(curr != max && curr > secondmax) {
			secondmax = curr;
		}
		temp /= 10;
	}
	printf("Second largest digit is: %d\n",secondmax);
}
