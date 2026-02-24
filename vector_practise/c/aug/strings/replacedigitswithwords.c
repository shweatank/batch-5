#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
char *p = NULL;
char* getstring() {
    int i = 0;
    char *p = NULL;
    do {
	    p = realloc(p,i+1);
	    p[i] = getchar();
    }while(p[i++] != '\n');
    p[i-1] = '\0';
    return p;
}

char * replaceDigitsWithWords(char *p) {
	char *str = p;
	char *digitwords[] = {"zero","one","two","three","four","five","six","seven","eight","nine"};
	int i = 0;
	while(str[i]) {
		int index = str[i] - '0';
		int len = strlen(str);
		int l = strlen(digitwords[index]);
		if(isdigit(str[i])) {
				str = realloc(str,len+l+1);
				memmove(str+i+l,str+i+1,len-i+1);
				strncpy(str+i,digitwords[index],l);
				i+=l;
		}
		else {
			i++;
		}
	}
	return str;
}

/*
char *replaceDigitsWithWords(char *str) {
    char *digitwords[] = {"zero","one","two","three","four","five","six","seven","eight","nine"};
    int i = 0;
    while (str[i]) {
        if (isdigit(str[i])) {
            int index = str[i] - '0';
            int len = strlen(str);
            int l = strlen(digitwords[index]);

            // reallocate enough space: old length + extra space + 1 for '\0'
            str = realloc(str, len + l);
            if (!str) {
                perror("realloc failed");
                exit(1);
            }

            // shift right part (include '\0')
            memmove(str + i + l, str + i + 1, len - i + 1);

            // copy word in place
            memcpy(str + i, digitwords[index], l);

            i += l; // move past inserted word
        } else {
            i++;
        }
    }
    return str;
}
*/
int main() {
	char *str;
	str = getstring();
	printf("Before: %s\n",str);
	str = replaceDigitsWithWords(str);
	printf("After: %s\n",str);
}	
