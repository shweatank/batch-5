#include <stdio.h>
#include <string.h>
#include <ctype.h>
void strCaseConversionAndReverseNumInStr(char * str);
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
	strCaseConversionAndReverseNumInStr(str);
	printf("%s\n",str);
}

void strCaseConversionAndReverseNumInStr(char * str) {
	int i = 0,j,k;
	while(str[i]) {
		if((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')) {
			str[i] ^= 32;
			i++;
		}
		else if(isdigit(str[i])) {
			j = i;
			while(isdigit(str[i])) {
				i++;
			}
			k = i-1;
			for(j,k;j<k;j++,k--) {
				char temp = str[j];
				str[j] = str[k];
				str[k] = temp;
			}
		}
		else i++;
	}
}

			
