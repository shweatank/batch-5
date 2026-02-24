#include<stdio.h>
#include<stdbool.h>
bool prime(int num) {
	int count = 0;
	if(num ==; 1) {
		return false;
	}
	for(int i=2;i<num/2;i++) {
		if(num%i == 0) {return false;}
	}
	return true;
}

void main() {
	int startRange,endRange;
	printf("Enter the starting number: ");
	scanf("%d",&startRange);
	printf("Enter the ending number: ");
	scanf("%d",&endRange);
	for(int i = startRange;i<=endRange;i++) {
		bool res = prime(i);
		if(res) {
			printf("%d ",i);
		}
	}
}



