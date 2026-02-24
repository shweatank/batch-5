#include "myheader.h"

student *deleteAtFirst(student *head) {
        student *temp = NULL;
        if(head == NULL) {
                printf("No list found\n");
        }
        else {
                temp = head->next;
                free(head);
        }
        return temp;
}
