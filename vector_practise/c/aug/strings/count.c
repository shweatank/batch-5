#include <stdio.h>
#include <string.h>
int count(char ch,char *arr);
int main() {
	char arr[20];
	scanf("%s",arr);
	for(char i = '0';i<='9';i++) {
		printf("%d ",count(i,arr));
	}
}
int count(char ch,char *arr) {
	int count = 0;
	for(int i = 0;i<strlen(arr)-1;i++) {
		//printf("--------------");
		if(arr[i] == ch) count++;
	}
	return count;
}
