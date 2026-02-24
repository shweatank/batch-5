#include <stdio.h>
#include <string.h>
#include <ctype.h>
char * longestWord(char *);
char * reverse(char *);
int main() {
        int size;
        printf("Enter the size: ");
        scanf("%d",&size);
        getchar();
        if(size < 0 || size > 100) {
                printf("Invalid size");
                return 0;
        }
        char str[size];
	//char sub[size];
        printf("Enter the String: ");
        scanf("%[^\n]s",str);
        //getchar();
        //printf("Enter the SubString: ");
        //scanf("%[^\n]s",sub);
	char *temp = str;
	char *res = longestWord(temp);
	printf("%s\n",res);
}

char * longestWord(char * str) {
	int maxlen = 0;
	char *max;
	char * s = strtok(str," ");
	while(s) {
		if(strlen(s) > maxlen) {
			maxlen = strlen(s);
			max = s;
		}
		s = strtok(NULL," ");
	}
	return reverse(max);
}

char * reverse(char * str) {
	int len = strlen(str);
	for(int i = 0,j=len - 1;i<j;i++,j--) {
		char temp = str[i];
		str[i] = str[j];
		str[j] = temp;
	}
	return str;
}

