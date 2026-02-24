#include "student_record.h"

int main() {
	student *head = NULL;
	char choice;
	while(1) {
		printf("****STUDENT RECORD MENU****\n");
		printf("                           \n");
       		printf("a / A    : add new record\n");
		printf("d / D    : delete a record\n");
		printf("s / S    : show a list\n");
		printf("m / M    : modify a record\n");
		printf("v / V    : save\n");
		printf("e / E    : exit\n");
		printf("t / T    : sort the list\n");
		printf("l / L    : delete all the records\n");
		printf("r / R    : reverse the list\n");
		printf("                           \n");
		printf("Enter your choice: ");
		scanf("%c",&choice);
		switch(choice) {
			case 'e':
			case 'E':
				exit(0);
			case 'a':
			case 'A':
//				head = stud_add(head);
				break;
			case 'd':
			case 'D':
//				head = stud_del(head);
				break;
			case 's':
			case 'S':
//				stud_show(head);
				break;
			case 'm':
			case 'M':
//				head = stud_mod(head);
				break;
		}
	}
}
