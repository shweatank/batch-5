#include "myheader.h"

student *insertAtFirst(student *head) {
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
                return head;
        }
        newnode->next = head;
        head = newnode;
        return head;
}
