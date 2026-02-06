#include<stdio.h>
#include<stdlib.h>
 struct node 
{
	int n;
	struct node*next;
};
struct node *Insert(struct node *);
void print(struct node *);
int main()
{
	struct node *head=NULL;
	char ch;
	while(1)
	{
		puts("enter i for insert");
		scanf(" %c",&ch);
		switch(ch)
		{
			case'i':head=Insert(head);
				break;
			case'p':print(head);
				break;
			case'e':exit(0);
		}
	}
}
void print(struct node *head)
{
	while(head)
	{
		printf("%d ",head->n);
		head=head->next;
	}
	puts("");
}
struct node *Insert(struct node *head)
{
	struct node *newnode=calloc(1,sizeof(struct node));
	if(newnode==NULL)
	{
		puts("memory is not allocated");
		return head;
	}
	puts("enter the data");
	scanf("%d",&newnode->n);
	if(head==NULL)
	{
		head=newnode;
		return head;
	}
	if(newnode->n<head->n)
	{
		newnode->next=head;
		head=newnode;
		return head;
	}
	struct node *temp=head,*temp2=NULL;
	while(temp)
	{
		if(newnode->n>temp->n)
		{
			temp2=temp;
			temp=temp->next;
		}
		else
		{
			temp2->next=newnode;
			newnode->next=temp;
			break;
		}
	}
	if(temp==NULL)
	{
		temp2->next=newnode;

	}
	return head;

}
