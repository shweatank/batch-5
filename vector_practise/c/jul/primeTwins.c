#include <stdio.h>
#include <math.h>

int isPrime(int num) {
	if(num == 0) return 0;
	if(num ==1 || num== 2) return 1;
	int s = sqrt(num);
	for(int i = 2;i<=s;i++) {
		if(num % i == 0) return 0;
	}
	return 1;
}
int main() {
	int min,max;
	printf("Enter the min range: ");
	scanf("%d",&min);
	printf("Enter the max range: ");
	scanf("%d",&max);
	for(;min <= max-2;min++) {
		if(isPrime(min) && isPrime(min+2)) {
			printf("(%d %d)",min,min+2);
		}
	}
	printf("\n");
}

