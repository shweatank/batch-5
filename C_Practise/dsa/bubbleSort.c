#include <stdio.h>

void *bubbleSort(int *arr,int len) {
	for(int i = 0;i<=len;i++) {
		for(int j = 0;j <= len;j++) {
			if(arr[j] > arr[j+1]) {
				int temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;
			}
		}
	}
	return arr;
}

int main() {
	int arr[] = {1,2,4,6,2,4,6,3,2,543,5,4,233,4,32,32,453,25,65,4,56,67,654,6,57,8,65,87,7,8,76,98,78,3425,5,643,5654,53,5,4356,345,546,343,35,54,7,54765,7,6587,66,547,56,76,7,86,5436,54,754,45,3654,654,6456,54654,6546,75,47,65743,62,5475,757,65375,65,765,7678,5786,5765,8765,765,876,765,67548654,865,8765,4765,765,4765,4765,4765,8,876,598,78,6,542,67,65387,658,764,32675,47,37534};
	int len = sizeof(arr)/sizeof(arr[0]);
	printf("%d\n",len);
	int *res = bubbleSort(arr,len);
	int i;
	for(i = 0;i<=len;i++) {
		printf("%d  ",res[i]);
	}
	printf("\n%d\n",i);
}
