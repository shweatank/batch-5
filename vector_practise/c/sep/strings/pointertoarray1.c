#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int count;
char (*)[20] insert(char (*p)[20]) {
	p = realloc(p,(count+1)*sizeof(*p));
	scanf("%s",p[count]);
	count++;
	return p;
}

int main() {
	char (*p)[20] = NULL;
	char  ch;
	int i = 0;
	while(1) {
	printf("Enter the choice: ");
	scanf("%c",&ch);
	if (ch == 'e')  exit(0);
	p = insert(p);
	printf("           %s\n       ",p[i++]);
	}
}
