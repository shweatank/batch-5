#include <stdio.h>
#include <string.h>
#include <ctype.h>
void nthWordCaseConversion(char *,int);
int main() {
        int size,n;
        printf("Enter the size: ");
        scanf("%d",&size);
        if(size < 0 || size > 100) {
                printf("Invalid size");
                return 0;
        }
        char str[size];
        printf("Enter the String: ");
        scanf(" %[^\n]s",str);
	printf("Enter the n: ");
	scanf("%d",&n);
	nthWordCaseConversion(str,n);

}

void nthWordCaseConversion(char * str,int n) {
	char *s = strtok(str," ");
	while(s) {
		if(n == 1) {
			for(int i = 0;s[i];i++) {
				s[i] ^= 32;
			}
		}
		printf("%s",s);
		s = strtok(NULL," ");
		(s) ? printf(" ") : printf("\n");
		n--;
	}
}

