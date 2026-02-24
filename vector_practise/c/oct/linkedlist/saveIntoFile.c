#include "myheader.h"

void saveIntoFile(student *head) {
	if (head != NULL) {
		FILE *fp = fopen("student.txt","w");
		if(fp == NULL) {
			printf("File creation failed\n");
			return;
		}
		int size = sizeof(student) - sizeof(head);//(to exclude the link address saved in file)
		//student temp;
		while(head) {
			//memcpy(&temp,head,size);
			//fwrite(&temp,size,1,fp);
			fprintf(fp,"%d %s\n",head->id,head->name);
			head = head->next;
		}
		fclose(fp);
	}
}
