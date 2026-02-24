#include "myheader.h"

student *insertAtLast(student *head) {
        student *newnode;
        newnode = calloc(1,sizeof(student));
        if(newnode == NULL) {
                printf("Node not created\n");
                exit(0);
        }
        printf("Enter the id: ");
        scanf("%d",&newnode->id);
        printf("Enter the name: ");
        scanf("%s",newnode->name);
        if(head == NULL) {
                head = newnode;
        }
        else {
                student *temp = head;
                while(temp->next) {
                        temp = temp->next;
                }
                temp->next = newnode;
        }
        return head;
}

void printStudents(student *head) {
        if(head == NULL) {
                printf("No students inserted\n");
                return;
        }
        do {
                printf("id : %d , name : %s\n",head->id,head->name);
                head = head->next;
        }while(head);
}

