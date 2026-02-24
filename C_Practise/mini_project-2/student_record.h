#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>

typedef struct student {
	int rollno;
	char name[20];
	float percentage;
	struct student *link;
}student;

student * stud_add(student *head);
