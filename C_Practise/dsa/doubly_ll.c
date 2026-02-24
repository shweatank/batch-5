#include <stdio.h>
#include <stdlib.h>

int count;
typedef struct node {
	struct node *head;
	int num;
	struct node *tail;
}node;

node *insertAtEnd(node *first) {
	node *n = calloc(1,sizeof(node));
	printf("Enter the num: ");
	scanf("%d",&n->num);
	if(first == NULL) {
		count++; 
		return n;
	}
	node *temp = first;
	while(temp->tail) {
		temp = temp->tail;
	}
	temp->tail = n;
	n->head = temp;
	count++;
	return first;
}

node *insertAtStart(node *first) {
	node *n = calloc(1,sizeof(node));
        printf("Enter the num: ");
        scanf("%d",&n->num);
	if(first == NULL) {
		count++; 
		return n;
	}
	n->tail = first;
	first->head = n;
	count++;
	return n;
}

node *insertAtN(node *first) {
	int pos;
	printf("Enter the position of node to be inserted: ");
	scanf("%d",&pos);
	if(pos > count) {
		printf("Invalid Position..! List has only %d nodes\n",count);
		return first;
	}
	node *n = calloc(1,sizeof(node));
        printf("Enter the num: ");
        scanf("%d",&n->num);
        if(first == NULL || pos == 0) {
		n->tail = first;
		first->head = n;
		count++; 
		return n;
	}
	node *temp = first;
	while(pos-1) {
		temp = temp->tail;
		pos--;
	}
	n->tail = temp->tail;
	temp->tail = n;
	n->head = temp;
	count++;
	return first;
}

node *insertBySorted(node *first) {
	node *n = calloc(1,sizeof(node));
        printf("Enter the num: ");
        scanf("%d",&n->num);
        if(first == NULL || n->num < first->num) {
		n->tail = first;
		first->head = n;
		count++;
		return n;
	}
	node *temp = first;
	while(temp->tail && temp->tail->num < n->num) {
		temp = temp->tail;
	}
	n->tail = temp->tail;
        temp->tail = n;
        n->head = temp;
        count++;
        return first;
}

node *deleteAtN(node *first) {
	int pos;
        printf("Enter the position of node to be deleted: ");
        scanf("%d",&pos);
        if(pos > count) {
                printf("Invalid Position..! List has only %d nodes\n",count);
                return first;
        }
	if(pos == 0) {
		first = first->tail;
		free(first->head);
		first->head = NULL;
		return first;
	}
	node *temp = first;
	while(pos) {
		temp = temp->tail;
		pos--;
	}
	temp->head->tail = temp->tail;
	if(temp->tail)	temp->tail->head = temp->head;
	free(temp);
	count--;
	return first;
}

node *deleteByData(node *first) {
	int num;
	printf("Enter the Data(num) to be deleted: ");
	scanf("%d",&num);
	node *temp = first;
	while(temp->num != num) {
		temp = temp->tail;
	}
	if(temp->head == NULL) {
		first = first->tail;
		free(first->head);
		first->head = NULL;
	}
	else {
		temp->head->tail = temp->tail;
		if(temp->tail)	temp->tail->head = temp->head;
		free(temp);
	}
	count--;
	return first;
}

node *reverse(node *first) {
	do {
		node *temp = first->head;
		first->head = first->tail;
		first->tail = temp;
		if(first->head)	first = first->head;
	}while(first->head);
	return first;
}	

void print(node *first) {
	while(first) {
		printf("%d\n",first->num);
		first = first->tail;
	}
}


int main() {
	int choice;
	node *first = NULL;
	while(1) {
		puts("1.insert at end");
		puts("2.insert at start");
		puts("3.insert at nth position");
		puts("4.insert at sorted order");
		puts("5.delete at nth position");
		puts("6.delete by data");
		puts("7.reverse the list");
		puts("8.print\n");
		
		printf("Enter the choice: ");
		scanf("%d",&choice);

		switch(choice) {
			case 1:
				first = insertAtEnd(first);
				break;
			case 2:
				first = insertAtStart(first);
				break;
			case 3:
				first = insertAtN(first);
				break;
			case 4:
				first = insertBySorted(first);
				break;
			case 5:
				first = deleteAtN(first);
				break;
			case 6:
				first = deleteByData(first);
				break;
			case 7:
				first = reverse(first);
				break;
			case 8:
				print(first);
				break;
			default:
				exit(0);
		}
	}
}

