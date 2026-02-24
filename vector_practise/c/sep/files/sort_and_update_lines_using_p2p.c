#include <string.h>
#include <stdio.h>
#include <stdlib.h>
int count = 0;
char ** readandstoredata(char *filename,char **data) {
	FILE *fp = fopen(filename,"r");
	if(fp == NULL) {
		printf("File not found\n");
		exit(0);
	}
	char str[100];
	data = NULL;
	while(fgets(str,100,fp)) {
		data = realloc(data,(count+1) * sizeof(char *));
		data[count] = malloc(strlen(str)+1);
		strcpy(data[count],str);
		count++;
	}
	fclose(fp);
	return data;
}

char ** sortbylen(char **data) {
	int i,j,k,l;
	for(i = 1;i<count;i++) {
		for(j = 0;j<count-i;j++) {
			k = strlen(data[j]);
			l = strlen(data[j+1]);
			if(k > l) {
				char *t = data[j];
				data[j] = data[j+1];
				data[j+1] = t;
			}
		}
	}
	return data;
}

char ** deletebyline(char **data) {
	int line;
	printf("Enter the line to be deleted: ");
	scanf("%d",&line);
	if(line < 1 || line > count) {
		printf("Invalid line number\n");
		exit(0);
	}
	memmove(data+line-1,data+line,sizeof(char *) * ((count)-line));
	count--;
	data = realloc(data,sizeof(char *)*count);
	return data;
}

void updatetofile(char *filename,char **data) {
	FILE *fp = fopen(filename,"w");
	int i = 0;
	while(i<count) {
		fputs(data[i],fp);
		i++;
	}
	
//	fwrite(data,sizeof(*data),count,fp);
	fclose(fp);
}

int main(int argc,char *argv[]) {
	if(argc != 2) {
		printf("Invalid arguments\n");
		exit(0);
	}
	char **data;
	data = readandstoredata(argv[1],data);
	//data = sortbylen(data);
	for(int i = 0;i<count;i++)
	printf("%s",data[i]);
	data = deletebyline(data);
	for(int i = 0;i<count;i++)
	printf("%s",data[i]);
//	updatetofile(argv[1],data);
}
