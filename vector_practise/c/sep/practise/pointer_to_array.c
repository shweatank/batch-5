#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>

int count = 0;
char *getstring() {
	char *str = NULL;
	int i = 0;
	do {
		str = realloc(str,(i+1)*sizeof(char));
		str[i] = getchar();
	}while(str[i++] != '\n');
	str[i] = '\0';
	return str;
}

char (* insert(char (*p)[20]))[20] {
	p = realloc(p,(count+1) * 20);
	printf("Enter the name: ");
	scanf("%s",p[count++]);	
	return p;
}

void* deletebyname(char (*p)[20]) {
	char name[20];
	printf("Enter the name to be deleted: ");
	scanf("%s",name);
	for(int i = 0;i<count;i++) {
		if(!strcmp(p[i],name)) {
			memmove(p+i,p+i+1,(count-i-1)*20);
			count--;
			p = realloc(p,count*20);
			return p;
		}
	}
	printf("Name not found\n");
	return p;
}

char (*sortbyname(char (*p)[20]))[20] {
	for(int i = 1;i<count;i++) {
		for(int j = 0;j<count-1;j++) {
			if(strcmp(p[j],p[j+1]) > 0) {
				char temp[20];
				strcpy(temp,p[j]);
				strcpy(p[j],p[j+1]);
				strcpy(p[j+1],temp);
			}
		}
	}
	return p;
}

void print(char (*p)[20]) {
	for(int i = 0;i<count;i++) {
		printf("%s\n",p[i]);
	}
}

int main() {
	char (*p)[20] = NULL;
	char choice;
	while(1) {
		printf("i.insert d.delete s.sort f.find p.print e.exit\n");
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
