#include <stdio.h>
#include <string.h>
void opposite_case_conversion(char *,char *);
char *mycasestrstr(char *str,char *substr);
int main() {
	char str[100],sub[100];
	printf("Enter the String: ");
	fgets(str,100,stdin);
	str[strchr(str,'\n') - str] = '\0';
	printf("Enter the SubString: ");
        fgets(sub,100,stdin);
	sub[strchr(sub,'\n') - sub] = '\0';
	char *p = mycasestrstr(str,sub);
	if(!p) {
		printf("Error");
		return 0;
	}
	opposite_case_conversion(str,sub);
	printf("%s",str);
}

char *mycasestrstr(char *str,char *substr) {
	int mainlen = strlen(str),i = 0,j = 0;
	int sublen = strlen(substr);
	for(i = 0;i<mainlen;) {
		if(j == sublen) break;
		if(str[i] == substr[j] || str[i] == (substr[j] ^ 32)) {
			i++;
			j++;
		}
		else {
			if(j != 0) j = 0;
			else i++;
		}
	}
	if(j == sublen) return str+i-j;
	return NULL;
}

void opposite_case_conversion(char *str,char *sub) {
	char *p = mycasestrstr(str,sub);
	while(p) {
		for(int i = 0;i<strlen(sub);i++) {
			 p[i] ^= 32;			 
		}
		p = mycasestrstr(p + strlen(sub),sub);
	}
}
