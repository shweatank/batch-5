#include <stdio.h>
#include <stdlib.h>

char *getstring() {
	char *ptr = realloc(NULL,1);
	int index = 0;
	char ch;
	while(1) {
		ch = getchar();
		if(ch == '\n') break;
		ptr[index++] = ch;
		ptr = realloc(ptr,index+1);
	}
	ptr[index] = '\0';
	return ptr;
}

