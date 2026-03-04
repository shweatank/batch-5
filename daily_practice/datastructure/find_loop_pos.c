
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdio_ext.h>
struct node
{
	int x;
	struct node *next;
};


struct node *Add(struct node *head)
{
	struct node*newnode=calloc(1,sizeof(struct node));
	if(newnode==NULL)
	{
		puts("memory not allocated");
		return 0;
	}
	puts("enter the data");
	scanf("%d",&newnode->x);
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

void print(struct node *head)
{
	while(head)
	{
		printf("%d ",head->x);
		head=head->next;
	}
	puts("");
}

bool find(struct node *head)
{
	struct node *slow,*fast;
	slow=fast=head;
	while(fast && fast->next)
	{
		slow=slow->next;
		fast=fast->next->next;
		if(slow==fast)
			return true;
	}
	return false;
}

void Makeloop(struct node *head)
{
	struct node*temp=NULL;
	int c=3;
	while(head->next)
	{
		head=head->next;
		if(c==0)
			temp=head;
		c--;
	}
	if(head->next==NULL)
		head->next=temp;

}

int main()
{
	struct node*head=NULL;
	char ch;
	while(1)
	{
		puts("enter the a add p print f find loop m make loop e exit");
		//__fpurge(stdin);
		scanf(" %c",&ch);
		switch(ch)
		{
			case 'a':head=Add(head);
				 break;
			case 'p':print(head);
				 break;
			case 'f':if(find(head))
					 printf("loop is find\n");
				 else
					 printf("loop is not find\n");
				 break;
			case 'm':Makeloop(head);
				 break;
			case 'e':exit(0);
		}
	}


}
