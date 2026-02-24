#include <stdio.h>
void print(char *,int);
int main() {
	char arr[20] = "1234asdfg*-+/qAs-/.,";
	print(arr,20);
}

void print(char *arr,int size) {
	int alpha = 0,digit = 0,special = 0;
	for(int i = 0;i<size;i++) {
		char ch = arr[i];
		if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) 
			alpha++;
		else if(ch >= '0' && ch <= '9')
			digit++;
		else special++;
	}
	printf("The characters in given array are: %d\n",alpha);
	printf("The digits in given array are: %d\n",digit);
	printf("The special characters in given array are: %d\n",special);
}
