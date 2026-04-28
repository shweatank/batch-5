#include<stdio.h>
#include<stdlib.h>
#include<stdio_ext.h>
typedef struct link_1
{
	int data;
	struct link_1 *link;
}link_list;
link_list *Add(link_list *top)
{
	link_list *next,*temp;
	next=calloc(1,sizeof(link_list));
	printf("enter the data:");
	scanf("%d",&next->data);
	if(top==NULL)
	{
		top=next;
		return top;
	}
	temp=top;
	while(temp->link)
	{
		temp=temp->link;
	}
	temp->link=next;
	return top;
}
void Print(link_list *top)
{
	if(top==NULL)
	{
		printf("list is empty\n");
	}
	while(top)
	{
		printf("data is:%d\n",top->data);
		top=top->link;
	}
}
link_list *Reverse(link_list *top)
{
	link_list *cur=NULL,*next=NULL,*prev=NULL;
	if(top==NULL)
	{
		printf("list is empty\n");
		return top;
	}
	next=top;
        while(next)
	{
		prev=cur;
		cur=next;
		next=next->link;
		cur->link=prev;
	}	
	top=cur;
	return top;
}/*
link_list *Delete(link_list *top)
{
	link_link *temp=NULL,*cur=NULL;
	int choice;
	if(top==NULL)
	{
		printf("list is empty\n");
		return NULL;
	}
	printf("enter the data to be delete\n");
	scanf("%d",&choice);
	if(top->data==choice)
	{
		top=top->link;
		return top;
	}
	temp=top;
	while(temp)
	{
		if(temp->data==choice)
			break;
		cur=temp;
		temp=temp->link;
	}
	if(temp)
	{
		cur->link=temp->link;
	}
	else
		printf("data not found\n");
	return top;
}
*/
link_list * delete_nth(link_list *head)
{
	link_list *fast,*slow;
	int n;
	printf("enter the nth :");scanf("%d",&n);
	if(!head)
	{
		printf("list is empty\n");
		return head;
	}
	fast=head;
	slow=head;
	while(n)
	{
		fast=fast->link;
		if(fast==NULL)
		{
			printf("n is greater\n");
			return head;
		}
		n--;
	}
	if(fast==NULL)
	{
		link_list *temp=head;
		head=head->link;
		free(head);
		return head;
	}
	while(fast->link!=NULL)
	{
		slow=slow->link;
		fast=fast->link;
	}
	link_list *temp=slow->link;
	slow->link=slow->link->link;
	free(temp);
	return head;
}
int main()
{
	link_list *top=NULL;
	char choice;
	while(1)
	{
		printf("enter the your choice:");__fpurge(stdin);scanf("%c",&choice);
		switch(choice)
		{
			case 'c':
				top=Add(top);
				break;
			case 'r':top=Reverse(top);
				break;
			case 'p':Print(top);
				 break;
			case 'd':top=delete_nth(top);
		}
	}
}
	
