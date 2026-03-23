#include<stdio.h>
#include<stdlib.h>
struct node
{
	int data;
	struct node *next;
};
void main()
{
struct node *newnode,*temp,*head=0;
newnode=(struct node*)malloc(sizeof(struct node));
printf("enter the data insert..\n");
scanf("%d",&newnode->data);
newnode->next=head;
head=newnode;
}

