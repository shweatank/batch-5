#include <stdio.h>
void main() {
	int num,res,temp;
	printf("Enter the number: ");
	scanf("%d",&num);
	if(num < 0) num *= -1;
	temp = num;
	res = 0;
loop:	if(temp) {
		res = res * 10 +(temp % 10);
		temp /= 10;
		goto loop;
	}
	(res == num) ? printf("Pallindome number\n") : printf("Not a pallindrome number\n");
}

