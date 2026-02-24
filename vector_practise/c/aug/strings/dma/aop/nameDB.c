//Array of pointers (rows fixed)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define max 10
int count = 0;
char * getstring() {
	char *p = NULL;
	int i = 0;
	do {
		p = realloc(p,i+1);
		p[i] = getchar();
	}while(p[i++] != '\n');
	p[i-1] = '\0';
	return p;
}

void insert(char **p) {
	if(count == max) {
		printf("DB is full");
		return;
	}
	p[count] = getstring();
	count++;
}

void print(char **p) {
	for(int i = 0;i<count;i++) {
		printf("%s\n",*(p+i));
	}
}

void deletebyname(char **p) {
	char *name;
	printf("Enter the name to be deleted: ");
	name = getstring();
	for(int i = 0;i<count;i++) {
		if(!strcmp(name,p[i])) {
			free(p[i]);
			memmove(p+i,p+i+1,(count-i-1)*(sizeof(*p)));
			count--;
		}
	}
	free(name);
}

void findbyname(char **p) {
	printf("Enter the name to find: ");
	char *name = getstring();
	for(int i = 0;i<count;i++) {
		if(!strcmp(p[i],name)) {
			printf("%s is found at %d index",name,i);
			return;
		}
	}
	printf("given name is not found");
}

void sortbyname(char **p) {
	for(int i = 1;i<count;i++) {
		for(int j = 0;j<count - i;j++) {
			if(strcmp(p[j],p[j+1]) > 0) {
				char *q = p[j];
				p[j] = p[j+1];
				p[j+1] = q;
			}
		}
	}
}

int main() {
	char option;
	char *nameDB[max];
	while(1) {
		printf("i.insert d.delete f.find p.print s.sort e.exit: ");
		scanf("%c",&option);
		getchar();
		switch(option) {
			case 'i':insert(nameDB);
				 break;
			case 'p':print(nameDB);
				 break;
			case 'd':deletebyname(nameDB);
				 break;
			case 'f':findbyname(nameDB);
				 break;
			case 's':sortbyname(nameDB);
				 break;
			case 'e':exit(0);
		}
	}
}
