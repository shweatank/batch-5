#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
typedef struct student
{
        int data;
        struct node *next;
}node;


void add_node(node**);
void print_node(node*);
void main()
{

        node *head=NULL;

        add_node(&head);
        add_node(&head);
        add_node(&head);
        add_node(&head);

        print_node(head);
}

void add_node(node **ptr)
{

        node *newnode;
        newnode=malloc(sizeof(node));

        printf("enter the data:\n");
        scanf("%d",&newnode->data);


        if(*ptr==NULL)
        {
                *ptr=newnode;
                newnode->next=*ptr;
        }
        else
        {
                node *temp;
                temp=*ptr;

                while(temp->next!=(*ptr))
                        temp=temp->next;


                temp->next=newnode;

                newnode->next=(*ptr);
        }

}

void print_node(node *ptr)
{
        if(ptr==NULL)
        {
                printf("no records are present:\n");
                return;
        }

        node *temp=ptr;

        do
        {
                printf("%d",temp->data);
                temp=temp->next;

        }while(temp!=ptr);
}

