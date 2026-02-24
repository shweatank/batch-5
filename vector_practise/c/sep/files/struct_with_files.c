#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int id;
	char name[20];
	float marks;
}STUDENT;

STUDENT * readDataFromFile(STUDENT *students,int *total,char *file) {
	FILE *fp = fopen(file,"r");
	if(fp == NULL) {
		printf("File not found\n");
		exit(0);
	}
	students = NULL;
	int count = 0;
	while(1) {
		students = realloc(students,(count+1)*sizeof(STUDENT));
		int res = fscanf(fp,"%d %s %f",&students[count].id,students[count].name,&students[count].marks);
		if(res != 3) break;
		count++;
	}
	*total = count;
	fclose(fp);
	return students;
}

int main(int argc,char *argv[]) {
	STUDENT *students;
	int total = 0;
	students = readDataFromFile(students,&total,argv[1]);
	for(int i = 0;i<total;i++) {
		printf("%d %s %f\n",students[i].id,students[i].name,students[i].marks);
	}
}
	
