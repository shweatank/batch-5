#include <stdio.h>
int swap(int);
int main() {
	int num = 0x12345678;
	num = swap(num);
	printf("%x\n",swap(num));
}

int swap(int num) {
	int i,j;
	char temp;
	char *p = (char *)&num;
	if(*p == 0x78) {
		for(i = 0,j = 3;i<j;i++,j--) {
			temp = *(p+i);
			*(p+i) = *(p+j);
			*(p+j) = temp;
		}
	}
	return num;
}
