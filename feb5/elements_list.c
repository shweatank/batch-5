#include<stdio.h>
#include<stdlib.h>
#include<stdio_ext.h>

typedef struct list
{
	int data;
	struct list *link;
}list;

void add(list** head)
{
	list *new=NULL,*temp=NULL,*prev=NULL;
	new=malloc(sizeof(list));
	if(!new)
		printf("memory allocation failure \n");
	else
	{
		printf("enter data  : ");
		scanf("%d",&new->data);
		if(*head==NULL)
		{
			*head=new;
		}
		else
		{
			temp=*head;
			while(temp &&(temp->data < new->data))
			{
				prev=temp;
				temp=temp->link;
			}
			prev->link=new;
			new->link=temp;
		}
	}
}

void print(list *head)
{
	while(head)
	{
		printf("%d ",head->data);
		head=head->link;
	}
	printf("\n");
}

int main()
{
	list *head=NULL;
	int choice;
	while(1)
	{
		printf("enter choice : \n");
		__fpurge(stdin);
		scanf("%d",&choice);	
		switch(choice)
		{
			case 1:add(&head);
				 break;
			case 2:print(head);
				 break;
			default :printf("invalid input\n");
		}
	}
}
