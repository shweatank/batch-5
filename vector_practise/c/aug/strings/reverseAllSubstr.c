#include <stdio.h>
#include <string.h>
#include <ctype.h>
void reverseAllOccurances(char *,char *);
int main() {
        int size,n;
        printf("Enter the size: ");
        scanf("%d",&size);
        if(size < 0 || size > 100) {
                printf("Invalid size");
                return 0;
        }
        char str[size],sub[size];
        printf("Enter the String: ");
        scanf(" %[^\n]s",str);
        printf("Enter the SubString: ");
        scanf(" %[^\n]s",sub);
	reverseAllOccurances(str,sub);
	printf("%s\n",str);
}

void reverseAllOccurances(char *str,char *sub) {
	char *s = strstr(str,sub);
	while(s) {
		int i = s-str;
		int j = (s-str) + strlen(sub)-1;
		for(i,j;i<j;i++,j--)  {
			char temp = str[i];
			str[i] = str[j];
			str[j] = temp;
		}
		s = strstr(s+strlen(sub),sub);
		//s = strstr(s++,sub);
	}
}

