#include <stdio.h>
#include <string.h>
void * mystrstr(char *main,char *sub);
int main() {
	char str[20],sub[20];
	scanf("%s %s",str,sub);
	char *p = mystrstr(str,sub);
	if(p) {
		printf("The given substring is starting at %ld index in %s\n",p-str,str);
	}
	else {
		printf("No substring present\n");
	}
}

void * mystrstr(char *main,char *sub) {
	int mainlen = strlen(main);
	int sublen = strlen(sub);
	int i = 0,j = 0;
	for(i;i<mainlen;) {
		if(j >= sublen) return main+(i-j);
		if(main[i] == sub[j]) {
			i++;
			j++;
		}
		else if( main[i] != sub[j] && j == 0) i++;
		else {
			j = 0;		
		}
	}
	return NULL;
}
