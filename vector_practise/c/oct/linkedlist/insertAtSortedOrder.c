#include "myheader.h"

student *insertAtSortedOrder(student *head) {
	student *newnode = calloc(1,sizeof(student));
	printf("Enter the id: ");
	scanf("%d",&newnode->id);
	printf("Enter the name: ");
	scanf("%s",newnode->name);
	if(head == NULL || head->id > newnode->id) {
		newnode->next = head;
		return newnode;
	}
	else {
		student *temp = head;
		while(temp->next != NULL && temp->next->id < newnode->id) {
			temp = temp->next;
		}
		newnode->next = temp->next;
		temp->next = newnode;

	}
	return head;
}

