#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int count;
char *getstring() {
	char *p = NULL;
	int i = 0;
	do{
		p = realloc(p,i+1);
		p[i] = getchar();
	}
	while(p[i++] != '\n');
	p[i-1] = '\0';
	return p;
}

char **insert(char **p) {
	p = realloc(p,(count+1)*sizeof(char*));	
	p[count] = getstring();
	count++;
	return p;
}

char **sortbyname(char **p) {
	for(int i = 1;i<count;i++) {
		for(int j = 0;j<count-i;j++) {
			if(strcmp(p[j],p[j+1]) > 0) {
				char *t = p[j];
				p[j] = p[j+1];
				p[j+1] = t;
			}
		}
	}
	return p;
}

void print(char **p) {
	for(int i = 0;i<count;i++) {
		printf("%s\n",p[i]);
	}
}

char **deletebyname(char **p) {
	printf("Enter the name: ");
	char *str = getstring();
	for(int i = 0;i<count;i++) {
		if(!strcmp(str,p[i])) {
			free(p[i]);
			memmove(p+i,p+i+1,(count-i-1)*sizeof(*p));
			count--;
			p = realloc(p,count * sizeof(char*));
		}
	}
	return p;
}

int main() {
	char **p = NULL;
	char choice;
	while(1) {
		printf("i.insert d.delete s.sort p.print f.find e.exit\n");
		printf("Enter the choice :");
		__fpurge(stdin);
		scanf("%c",&choice);
		__fpurge(stdin);
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
