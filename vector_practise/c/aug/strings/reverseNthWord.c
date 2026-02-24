#include <stdio.h>
#include <string.h>
#include <ctype.h>
void reverseNthWord(char *,int);
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
	reverseNthWord(str,n);
}

void reverseNthWord(char * str,int n) {
	char *s = strtok(str," ");
	while(s) {
		if(n == 1) {
			for(int i = 0,j = strlen(s) - 1;i<j;i++,j--) {
				char temp = s[i];
				s[i] = s[j];
				s[j] = temp;
				i++;
				j--;
			}
		}
		printf("%s",s);
		s = strtok(NULL," ");
		(s) ? printf(" ") : printf("\n");
		n--;
	}
}

