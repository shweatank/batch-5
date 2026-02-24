#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdio_ext.h>
int count;

char * getstring() {
	char *p = NULL;
	int count = 0;
	do {
		p = realloc(p,count+1);
		p[count] = getchar();
	}while(p[count++] != '\n');
	return p;
}

void * insert(char (*p)[20]) {
	p = realloc(p,(count+1) * sizeof(*p));
	scanf("%s",p[count]);
	count++;
	return p;
}

void *deletebyname(char (*p)[20]) {
	char name[20];
	printf("Enter the name to be deleted ");
	scanf("%s",name);
	for(int i = 0;i<count;i++) {
		if(!strcmp(name,p[i])) {
				memmove(p+i,p+i+1,(count - i -1) * sizeof(*p));
				count--;
				p = realloc(p,(count)*sizeof(*p));
				return p;
		}
	}
	printf("%s name is not found",name);
	return p;
}

void *sortbyname(char (*p)[20]) {
	char * t;
	for(int i = 1;i<count;i++) {
		for(int j = 0;j<count-i;j++) {
			if(strcmp(p[j],p[j+1]) > 0) {
				strcpy(t,p[j]);
				strcpy(p[j],p[j+1]);
				strcpy(p[j+1],t);
			}
		}
	}
	return p;
}

void print(char (*p)[20]) {
	for(int i = 0;i<count;i++)
		printf("%s\n",p[i]);
}

int main() {
	char (*p)[20]=NULL;
	char choice;
	while(1) {
		printf("i.insert d.delete f.find s.sort p.print e.exit\n");
		printf("Enter the choice: ");
		__fpurge(stdin);
		scanf("%c",&choice);
		switch(choice) {
			case 'i':
				p = insert(p);
				break;
			case 'p':
				print(p);
				break;
			case 'd':
				p = deletebyname(p);
				break;
			case 's':
				p = sortbyname(p);
				break;
			case 'e':
				exit(0);
		}
	}
}

