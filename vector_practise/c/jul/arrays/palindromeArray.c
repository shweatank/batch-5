#include <stdio.h>
int palindromeArray(int *,int);
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
        palindromeArray(arr,size) ? printf("Palindrome Array\n") : printf("Not a Palindrome Array\n");
}

int palindromeArray(int * arr,int size) {
	int i,j;
	for(i = 0,j = size-1;i<j;i++,j--) {
		if(arr[i] != arr[j]) return 0;
	}
	return 1;
}

