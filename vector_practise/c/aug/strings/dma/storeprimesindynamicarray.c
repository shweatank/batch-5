#include <stdio.h>
#include <math.h>
#include <stdlib.h>
int count;
int isPrime(int num) {
	int sqt = sqrt(num);
	if(num < 2 || num % 2 == 0) return 0;
	if(num == 2) return 1;
	for(int i = 3;i<=sqt;i++) {
		if(num % i == 0) return 0;
	}
	return 1;
}

void primesArray(int min,int max,int **arr) {
	for(min;min<=max;min++) {
		if(isPrime(min)) {
			*arr = realloc(*arr,(count+1)*sizeof(min));
			(*arr)[count] = min;
			count++;
		}
	}
}

void printArray(int *arr) {
	for(int i = 0;i<count;i++) {
		printf("%d,",arr[i]);
	}
}

int main() {
	int min,max;
	printf("Enter the min range: ");
	scanf("%d",&min);
	printf("Enter the max range: ");
        scanf("%d",&max);
	int *arr = NULL;
	primesArray(min,max,&arr);
	printArray(arr);
}
