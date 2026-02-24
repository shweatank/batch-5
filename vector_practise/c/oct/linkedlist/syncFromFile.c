#include "myheader.h"

student* syncFromFile(student *head) {
	char filename[20];
	printf("Enter the data filename: ");
	scanf("%s",filename);
	FILE *fp = fopen(filename,"r");
	if(fp == NULL) {
		printf("File not found\n");
		return head;
	}
	student *newnode;
	student temp;
	while(fscanf(fp,"%d %s",&temp.id,temp.name) == 2) {
		newnode = calloc(1,sizeof(student));
		memcpy(newnode,&temp,sizeof(student) - sizeof(head));
		if(head == NULL) {
			head = newnode;
		}
		else {
			student *temp2 = head;
			while(temp2->next) {
				temp2 = temp2->next;
			}
			temp2->next = newnode;
		}
	}
	fclose(fp);
	return head;
}
