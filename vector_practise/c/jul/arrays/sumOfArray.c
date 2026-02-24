#include <stdio.h>
int sum(int *,int);
extern void inputArray(int *,int);
extern void printArray(int *,int);

int main() {
	int size;
        printf("Enter the size of Array: ");
        scanf("%d",&size);
        int arr[size];
        inputArray(arr,size);
        printf("The elements in array are: ");
        printArray(arr,size);
        printf("\n");
        printf("The sum of Array is: %d\n",sum(arr,size));
}

int sum(int * arr,int size) {
	int i,sum = 0;
	for(i = 0;i<size;i++) {
		sum+=arr[i];
	}
	return sum;
}







