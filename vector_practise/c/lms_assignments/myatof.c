#include <stdio.h>
#include <string.h>
#include <ctype.h>

float myatof(char * str) {
	int len = strlen(str);
	int i = 0,sign = 1,index = 0;
	float res = 0.0;
	char *p = strchr(str,'.');
	if(str[0] == '-') {
		sign = -1;
		i++;
	}
	if(str[len-1] == '.' || p == NULL) {
		return 0.0;
	}
	index = p - str;
	int mul = 1;
	for(i;i<index;i++) {
		if(!(str[i] >= '0' && str[i] <= '9')) {
			return 0.0;
		}
		res = res*mul + (str[i] - '0');
		mul = 10;
	}
	mul = 10;
	index++;
	for(index;index<len;index++) {
		if(!(str[index] >= '0' && str[index] <= '9')) {
			return 0.0;
		}
		res = res + (str[index] - '0')/mul;
		mul*=10;
	}
	return res*sign;
}

int main() {
	char str[] = "123.234";
	float res = myatof(str);
	printf("%f",res);
}


