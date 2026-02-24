#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
void input(int *,int);
void frequency(int *,int);
int main() {
	int arr[20];
	input(arr,20);
	frequency(arr,20);
}

void input(int * arr,int size) {
	srand(getpid());
	for(int i = 0;i<size;i++) {
		arr[i] = rand()%5;
	}
}

void frequency(int * arr,int size) {
	int count,flag;
	for(int i = 0;i<size;i++) {
		count = 0,flag = 1;
		for(int k = 0;k<i;k++) {
			if(arr[k] == arr[i]) flag = 0;
		}
		if(flag) {
			for(int j = 0;j<size;j++) {
				if(arr[i] == arr[j]) {
					count++;
				}
			}
		}
		if(flag) 
			printf("The frequency of %d number in given array is: %d\n",arr[i],count);
	}
}
