#include <stdio.h>
#include <string.h>
#include <ctype.h>
void allSubStr(char * str,char *sub);
void toUppercase(char *,int);
int main() {
	int size;
	printf("Enter the size: ");
	scanf("%d",&size);
	getchar();
	if(size < 0 || size > 100) {
		printf("Invalid size");
		return 0;
	}
	char str[size],sub[size];
	printf("Enter the String: ");
	scanf("%[^\n]s",str);
	getchar();
	printf("Enter the SubString: ");
	scanf("%[^\n]s",sub);
	char *p = strstr(str,sub);
        if(!p) {
                printf("No substring found\n");
                return 0;
        }
	allSubStr(str,sub);
	printf("%s\n",str);
}

void allSubStr(char * str,char * sub) {
	char *p = strstr(str,sub);
        while(p) {
                toUppercase(p,strlen(sub));
                p = strstr(p,sub);
        }
}

void toUppercase(char *str,int size) {
	while(*str && size) {
		if(*str >= 'a' && *str <= 'z') {
			*str -= 32;
		}
		size--;
		str++;
	}
}
