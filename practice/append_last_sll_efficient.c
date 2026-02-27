#include<stdio.h>
#include<stdlib.h>
typedef struct node
{
	int d;
	struct node *link;
}NODE;
typedef struct linkedlist
{
	NODE*head;
	NODE *tail;
}list;
void append_last(list *ptr,int da)
{
	NODE *newnode=calloc(1,sizeof(NODE));
	newnode->d=da;
	if(ptr->head==NULL)
	{
		ptr->head=ptr->tail=newnode;
	}
	else
	{
		ptr->tail->link=newnode;
		ptr->tail=newnode;
	}
}
void print(NODE *ptr)
{
	while(ptr)
	{
		printf("%d ",ptr->d);
		ptr=ptr->link;
	}
}
int main()
{
	int ch,t;
	list var;
	while(1)
	{
	printf("choice:");scanf("%d",&ch);
	switch(ch)
	{
		case 1:printf("data:");scanf("%d",&t);
		       append_last(&var,t);break;
		case 2:print(var.head);break;
		case 3:exit(0);
	}
	}
	return 0;
}
