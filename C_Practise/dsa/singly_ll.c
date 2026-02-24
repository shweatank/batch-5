#include <stdio.h>
#include <stdlib.h>

int count = 0;
typedef struct node {
	int num;
	struct node *link;
}node;

node *insertAtEnd(node *head) {
	int num;
	printf("Enter the num to store: ");
	scanf("%d",&num);
	if(head == NULL) {
		head = malloc(sizeof(node));
		head->num = num;
		head->link = NULL;
	}
	else {
		node *temp = head;
		while(temp->link) temp = temp->link;
		temp->link =malloc(sizeof(node));
		temp->link->num = num;
		temp->link->link = NULL;
	}
	count++;
	return head;
}

node *insertAtFirst(node *head) {
	node *n = malloc(sizeof(node));
	printf("Enter the num: ");
	scanf("%d",&n->num);
	n->link = NULL;
	if(head != NULL) n->link = head;
	count++;
	return n;
}

node *insertAtN(node *head) {
	int pos;
	printf("Enter the position of data to be inserted: ");
	scanf("%d",&pos);
	if(pos > count) {
		printf("Invalid position! List has only %d nodes\n",count);
		return head;
	}
	node *n = calloc(1,sizeof(node));
	printf("Enter the num: ");
	scanf("%d",&n->num);
	node *temp = head;
	int i = 0;
	while(i < (pos - 1)) {
		temp = temp->link;
		i++;
	}
	node *t = temp->link;
	temp->link = n;
	n->link = t;
	count++;
	return head;
}

node *insertAtSorted(node *head) {
	node *n = calloc(1,sizeof(node));
	printf("Enter the num: ");
        scanf("%d",&n->num);
	if(head == NULL) head = n;
	else if(head->num > n->num) {
		n->link = head;
		count++;
		return n;
	}
	else {
		node *temp = head;
		while(temp->num < n->num && temp->link->num < n->num) {
			temp = temp->link;
		}
		n->link = temp->link;
		temp->link = n;
		count++;
	}
	count++;
	return head;
}

node *deleteAtN(node *head) {
	int pos;
	printf("Enter the position to be deleted: ");
	scanf("%d",&pos);
	if(pos > count) {
		printf("Invalid position..! List only has %d nodes\n",count);
		return head;
	}
	node *temp = head;
	if(pos == 0) {
		head = head->link;
                free(temp);
                count--;
		return head;
	}
	while((pos-1)) {
		temp = temp->link;
		pos--;
	}
	node *t = temp->link;
	temp->link = temp->link->link;
	free(t);
	count--;
	return head;
}

node *deleteByData(node *head) {
	int num;
	printf("Enter the num to be deleted: ");
        scanf("%d",&num);
	node *temp = head;
	if(temp->num == num) {
                head = head->link;
                free(temp);
                count--;
	}
	else {
		while(temp->link && temp->link->num != num) {
			temp = temp->link;
		}
		if(temp->link) {
			node *t = temp->link;
			temp->link = temp->link->link;
			free(t);
			count--;
		}
		else {
			puts("The num is not present in list..!");
		}
	}
	return head;
}

void print(node *head) {
	while(head) {
		printf("%d\n",head->num);
		head = head->link;
	}
}

int main() {
	node *head = NULL;
	int choice;
	while(1) {
		puts("1.insert at end");
		puts("2.insert at first");
		puts("3.insert at nth position");
		puts("4.insert at sorted order");
		puts("5.delete at nth position");
		puts("6.delete by data");
		puts("7.print");
		puts("Enter the choice: ");
		scanf("%d",&choice);
		switch(choice) {
			case 1:
				head = insertAtEnd(head);
			       	break;
			case 2:
				head = insertAtFirst(head);
			        break;
			case 3:
				head = insertAtN(head);
				break;
			case 4:
				head = insertAtSorted(head);
				break;
			case 5:
				head = deleteAtN(head);
				break;
			case 6:
				head = deleteByData(head);
				break;
			case 7:
				print(head);
				break;
			default:
			       exit(0);
		}
	}
}

