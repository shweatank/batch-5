#include <string.h>
#include <stdio.h>

int count = 0;
char *getstring() {
	char *str = malloc(1);
	int i = 0;
	char ch = getchar();
	while(ch != '\n') {
		str[i++] = ch;
		str = realloc(str,i+1);
		ch = getchar();
	}
	str[i] = '\0';
	return str;
}

char * insert(char (*arr)[20]) {
	arr = realloc(arr,(count+1)*sizeof(*arr));
	printf("Enter the name: ");
	scanf("%s",arr[count]);
	count++;
	return arr;
}

char *deleteByName(char (*arr)[20]) {
	int flag = 1;
	char *name = malloc(sizeof(*arr));
	printf("Enter the name to be deleted: ");
	scanf("%s",name);
	for(int i = 0;i<count;i++) {
		if(strcmp(name,arr[i]) == 0) {
			memmove(arr+i,arr+i+1,(count-i-1)*sizeof(*arr));
			count--;
			flag = 0;
		}
	}
	if(flag) printf("No name found! Delete failed..");
	return arr;
}

