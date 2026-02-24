#include <stdio.h>
extern void inputArray(int *,int);
extern void printArray(int *,int);
void reverse(int *,int);
int main() {
	int size;
	printf("Enter the size of Array: ");
	scanf("%d",&size);
	int arr[size];
	inputArray(arr,size);
	printf("The elements in array are: ");
	printArray(arr,size);
	printf("\n");
	reverse(arr,size);
	printf("The elements in array after reversing are: ");
	printArray(arr,size);
	printf("\n");
}

void reverse(int *arr,int size) {
	int i,j,temp;
	for(i = 0,j = size-1;i<j;i++,j--) {
		temp = arr[i];
		arr[i] = arr[j];
		arr[j] = temp;
	}
}
