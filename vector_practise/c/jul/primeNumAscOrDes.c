#include <stdio.h>
#include <math.h>
int ascending(int num) {
	if(num < 10) return 1;
	int last = num%10;
	while(num) {
		int curr = num % 10;
		if(curr > last) return 0;
		else last = curr;
		num /= 10;
	}
	return 1;
}
int descending(int num) {
	if(num < 10) return 1;
        int last = num%10;
        while(num) {
                int curr = num % 10;
                if(curr < last) return 0;
		else last = curr;
                num /= 10;
        }
        return 1;
}
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
	for(;min <= max;min++) {
		if(isPrime(min) && (ascending(min) || descending(min))) {
			printf("%*d ",4,min);
		}
	}
	printf("\n");
}

