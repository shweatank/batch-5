#include<stdio.h>
int main()
{
	int arr[10]={1,2,2,2,1,3,4,3,4,6},n=10;
	 for (int i = 0; i < n; i++) {
        int index = arr[i] % n;  // get original value
        arr[index] += n;

        // If this is the second time we incremented, it is repeated
        if ((arr[index] / n) == 2) {
            printf("%d ", index);
        }
    }
	 printf("\n");
}
