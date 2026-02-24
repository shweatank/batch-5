#include <stdio.h>
int main() {
	int num;
	scanf("%d",&num);
	while(num) {
		int temp1 = num % 10;
		int count = 0;
		int n = num;
		while(n) {
			int temp2 = num % 10;
			if(temp1 == temp2) count++;
			n/=10;
		}


