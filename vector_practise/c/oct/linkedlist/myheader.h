#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>

typedef struct student {
        int id;
        char name[20];
        struct student *next;
}student;

student *insertAtLast(student *);
student *insertAtFirst(student *);
student *deleteAtFirst(student *);
void printStudents(student *);
void saveIntoFile(student *);
student* syncFromFile(student *);
student *insertAtSortedOrder(student *);

