#include<stdio.h>
void inputArray(int * arr,int size) {
	for(int i = 0;i<size;i++) {
		printf("Enter the %d element: ",i);
		scanf("%d",arr+i);
	}
}

void printArray(int * arr,int size) {
	for(int i = 0;i<size;i++) {
		printf("%d,",arr[i]);
	}
}
