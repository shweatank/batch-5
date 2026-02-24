#include <stdio.h>
int swap(short int num);
int main() {
	short int num;
	printf("Enter the number: ");
	scanf("%hd",&num);
	printf("After converting to big-Endian format the answer is: %hd\n",swap(num));
}

int swap(short int num) {
	int i,j;
	char *p = (char *)&num;
	for(i = 0,j=1; i < j;i++,j--) {
		char temp = *(p+i);
		*(p+i) = *(p+j);
		*(p+j) = temp;
	}
	return num;
}

