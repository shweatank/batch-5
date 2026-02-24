#include <stdio.h>
#include <string.h>
float atof(char *str);
int main() {
	char str[20];
	printf("Enter the string :");
	scanf("%s",str);
	char ch = str[0];
	float res = atof(str);
	printf("%f\n",res);
}

float atof(char *str) {
	float num = 0,mul = 10;
	int len = strlen(str),index = strchr(str,'.')-str,flag = 1;
	char ch = str[0];
	if(ch =='-') {
		flag = -1;
		str++;
	}
	else if(ch == '+' || ch == '\t' || ch == '\n' || ch == ' ') {
		str++;
        }
	for(int i = 0;i < len;i++) {
		if(i == index) continue;
		if(str[i] < '0' || str[i] > '9') return num;
		if(i < index) {
			num = num*10 + (str[i] - '0');
		}
		else {
			num = num + ((str[i] - '0') / mul);
			mul *= 10;
		}
	}
	return num*flag;
}
