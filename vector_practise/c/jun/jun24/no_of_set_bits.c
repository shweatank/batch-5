#include <stdio.h>
int countSetBits(int num);
int main() {
	int num;
	printf("Enter the number: ");
	scanf("%d",&num);
	printf("The number of set bits in  given number is: %d\n",countSetBits(num));
}
int countSetBits(int num) {
	int count = 0;
//	while(num) {
//		int curr = num % 2;
//		if(curr == 1) count++;
//		num /= 2;
//	}
//
//	for(int i = 0;i<=31;i++) {
//		int sum = 1 << i;
//		if(sum & num) count++;
//		if(sum >= num) break;
//	}
	int var = 0;
	while((1 << var) <= num) {
		if((1 << var) & num) count++;
		var++;
	}
	return count;
}
