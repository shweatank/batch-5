#include <stdio.h>
int main() {
	int num,temp,count;
	printf("Enter the number:");
	scanf("%d",&num);
	temp = num;
	count = 0;
loop:	if(temp) {
		count += 1;
		temp /= 10;
		goto loop;
	}
	printf("Number of digits are: %d\n",count);
}
