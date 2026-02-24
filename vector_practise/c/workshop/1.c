#include <stdio.h>
#include <math.h>
void inputArray(int * arr,int size);
int maxPrime(int *arr,int size);
int isPrime(int num);
int main() {
	int size;
	printf("Enter the size of array: ");
	scanf("%d",&size);
	if(size <= 0 || size > 100) {
		printf("Invalid size");
		return 0;
	}
	int arr[size];
	inputArray(arr,size);
	int max = maxPrime(arr,size);
	(max > 0) ? printf("The max prime number in given array is: %d\n",max) : printf("No prime numbers\n");
}
int maxPrime(int *arr,int size) {
	int max = -1;
	for(int i = 0;i<size;i++) {
		if(arr[i] > 0 && isPrime(arr[i]) && arr[i] > max) {
			max = arr[i];
		}
	}
	return max;
}

int isPrime(int num) {
	if(num == 2) return num;
	int n = sqrt(num);
	for(int i = 2;i<=n;i++) {
		if(num % i == 0) return 0;
	}
	return 1;
}

void inputArray(int * arr,int size) {
        for(int i = 0;i<size;i++) {
                printf("Enter the %d element: ",i);
                scanf("%d",arr+i);
        }
}


