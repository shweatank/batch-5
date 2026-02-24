#include <stdio.h>
#include <string.h>
#include "getstring.c"

int main() {
	printf("Enter the main string: ");
	char *str = getstring();
	printf("Enter the sub string: ");
	char *substr = getstring();
	if(strlen(substr) > strlen(str)) {
		printf("Invalid substring\n");
		return 0;
	}
	char *ptr = strstr(str,substr);
	while(ptr) {
		for(int i = 0;i<strlen(substr);i++) {
			int t = (ptr-str)+i;
			if(str[t] >= 'a' && str[t] <= 'z') {
				str[t] -= 32;
			}
		}
		ptr = strstr(ptr,substr);
	}
	printf("%s\n",str);
}
