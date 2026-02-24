#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct node
{
	int data;
	struct node *link;
};

struct node* add_node(struct node *head)
{
	struct node *new=NULL;
	new=malloc(sizeof(*head));
	if(!new)
	{
		printf("memory allocation failure\n");
		return head;
	}
	new->data=rand()%20;
	new->link=head;
	head=new;
	return head;
}

void print_nodes(struct node* head)
{
	while(head)
	{
		printf("data : %d\n",head->data);
		head=head->link;
	}
}

struct node* remove_node(struct node*head,int pos)
{
	struct node *slow,*fast,*temp;
	slow=head;
	fast=head;

	for(int i=0;i<pos;i++)
		fast=fast->link;
	while(fast->link)
	{
		fast=fast->link;
		slow=slow->link;
	}

	temp=slow->link;
	slow->link=slow->link->link;
	free(temp);
	return head;

}

int main()
{
	struct node *head=NULL;
	for(int i=0;i<5;i++)
		head=add_node(head);
	print_nodes(head);
	head=remove_node(head,4);
	print_nodes(head);
	return 0;
}
