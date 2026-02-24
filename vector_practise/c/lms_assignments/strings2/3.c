#include <stdio.h>
#include <ctype.h>
#include <string.h>
int bintoint(char *str);
int main() {
	int size;
	printf("Enter the size: ");
	scanf("%d",&size);
	getchar();
	char str[size];
	printf("Enter the string: ");
	scanf("%s",str);
	int res = bintoint(str);
	(res != 0) ? printf("The given binary %s is equialent to %d\n",str,res) : printf("Invalid input\n");
}

int bintoint(char *str) {
	int res = 0,mul = 1,i = strlen(str) - 1;
	for(i;i>=0;i--) {
		if(str[i] != '0' && str[i] != '1') return 0;
		res += (str[i] -'0')*mul;
		mul *= 2;
	}
	return res;
}
