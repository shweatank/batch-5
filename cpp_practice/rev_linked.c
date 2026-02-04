#include<stdio.h>
#include<stdlib.h>
#include<stdio_ext.h>

typedef struct list
{
	int data;
	struct list *link;
}list;


list* reverse_list(list* head)
{
	list *next,*cur,*prev;
	prev=next=NULL;
	cur=head;
	while(cur!=NULL)
	{
		next=cur->link;
		cur->link=prev;
		prev=cur;
		cur=next;
	}
	return prev;
}

void print_list(list* head)
{
	while(head)
	{
		printf("%d\t",head->data);
		head=head->link;
	}
	printf("\n");
}

list* add_node(list* head)
{
	list *new=NULL;
	new=malloc(sizeof(list));
	if(new==NULL)
	{
		printf("memory allocation failure\n");
		return head;
	}
	printf("enter data : ");
	scanf("%d",&new->data);
	new->link=head;
	head=new;
	return head;
}

int main()
{
	list *head=NULL;
	char choice;
	while(1)
	{
		printf("enter choice A:add_node R:reverse_list P: Print_nodes\n");
		__fpurge(stdin);
		scanf("%c",&choice);

		switch(choice)
		{
			case 'A':head=add_node(head);
				 break;
			case 'P':print_list(head);
				 break;
			case 'R':head=reverse_list(head);
				 break;
		}
	}
}
