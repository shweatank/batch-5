#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int palindrome(char *str,int index) {
    int len  = strlen(str);
    int flag = 1;
    for(int i = 0,j = len - 1;i<j;i++,j--) {
	    if(str[i] != str[j]) {
		   flag = 0;
		   break;
	    }
    }
    if(flag) return -1; 
    for(int i = 0,j = len - 1;i<j;i++,j--) {
        if(index > len/2) j--;
        else i++;
        if(str[i] != str[j]) return 0;
    }
    return 1;
}

int main() {
    char str[100];
    scanf("%[^\n]s",str);
    for(int i = 0;i<strlen(str);i++) {
        int res = palindrome(str,i);
        if(res == -1) {
            printf("-1");
            exit(0);
        }
        else if(res == 1) {
            printf("%d",i);
            exit(0);
        }
    }
    printf("can not be a palindrome");
}
