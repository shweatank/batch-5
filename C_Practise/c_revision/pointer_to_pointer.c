#include <stdio.h>
#include <string.h>
#include <getstring.c>

int count = 0;
char ** insert(char **ptr) {
	ptr = realloc(ptr,(count+1)*sizeof(*ptr));
	ptr[count] = getstring();
	count++;
	return ptr;
}


