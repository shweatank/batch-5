#include <stdio.h>
#include <math.h>
int main() {
	int num,count,res,temp;
	printf("Enter the number: ");
	scanf("%d",&num);
	temp = num;
	res,count = 0;
loop:	if(temp) {
		count++;
		temp /= 10;
		goto loop;
	}
	temp = num;
loop2:	if(temp) {
		int curr = temp % 10;
		int val = pow(curr,count);
		res += val;
		temp /= 10;
		goto loop2;
	}
	(res == num) ? printf("Armstrong number\n") : printf("Not a Armstrong number\n");
}


