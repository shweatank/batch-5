#include<stdio.h>
#include<stdlib.h>
int cnt;
struct node
{
	int data;
	struct node *next;
};

struct node* Insert(struct node *head)
{
	struct node *newnode=NULL;
	newnode=calloc(1,sizeof(struct node));
	if(newnode==NULL)
	{
		puts("not allocates memory");
		return head;
	}
	printf("enter the number\n");
	scanf("%d",&newnode->data);
	cnt++;
	if(head==NULL)
	{
		head=newnode;
	}
	else
	{
		newnode->next=head;
		head=newnode;
	}
	return head;
}


void Find_middle(struct node *head)
{
	int t=cnt;
	t=t/2;
	
	while(head&&t)
	{
		head=head->next;
		t--;
	}
	printf("%d \n",head->data);
}

void Print(struct node *head)
{
	while(head)
	{
		printf("%d ",head->data);
		head=head->next;
	}
	printf("\n");
}



int main()
{
	struct node*head=NULL;
	char ch;
	while(1)
	{
		puts("i insert f find p print e exit");
		scanf(" %c",&ch);
		switch(ch)
		{
			case'i':head=Insert(head);
				break;
			case'f':Find_middle(head);
				break;
			case'p':Print(head);
				break;
			case'e':exit(0);
		}
	}
}
