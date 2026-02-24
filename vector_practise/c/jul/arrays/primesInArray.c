#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
int isPrime(int);
void input(int *,int);
void print(int *,int);
int main() {
	int size;
	printf("enter the size of array: ");
	scanf("%d",&size);
	int arr[size];
	input(arr,size);
	print(arr,size);
}

void input(int *arr,int size) {
	srand(getpid());
	for(int i = 0;i<size;i++) {
		arr[i] = rand()%101;
	}
}

void print(int *arr,int size) {
	for(int i = 0;i<size;i++) {
		if(isPrime(arr[i])) {
			printf("%d,",arr[i]);
		}
	}
}

int isPrime(int num) {
	int sqr = sqrt(num);
	for(int i = 2;i<=sqr;i++) {
		if(num % i == 0) return 0;
	}
	return 1;
}

