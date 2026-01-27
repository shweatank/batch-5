#include<stdio.h>
#include<stdlib.h>
#include<stdio_ext.h>
struct node
{
	int data;
	struct node *next;
};


struct node *push(struct node *top)
{
	struct node *newnode=calloc(1,sizeof(struct node));
	if(newnode==NULL)
	{
		puts("memory is not allocated");
		return top;
	}
	printf("enter the data\n");
	scanf("%d",&newnode->data);
	if(top==NULL)
	{
		top=newnode;
		return top;
	}
	newnode->next=top;
	top=newnode;
	return top;
}
struct node*pop(struct node*top)
{
	if(top==NULL)
	{
		puts("stack is underflow");
		return top;
	}
	struct node *temp;
	temp=top;
	top=top->next;
	free(temp);
	temp=NULL;
	return top;
}
void peek(struct node *top)
{
	if(top==NULL)
	{
		puts("no peek element");
		return;
	}
	puts("peek element is");
	printf("%d\n",top->data);
}

void display(struct node *top)
{
	if(top==NULL)
	{
		puts("no elements in stack");
		return;
	}
	while(top)
	{
		printf("%d ",top->data);
		top=top->next;
	}
	puts("");
}

int main()
{
	struct node *top=NULL;
	char ch;
	while(1)
	{
		puts("enter i push d pop t peek p display e exit");
		__fpurge(stdin);
		scanf("%c",&ch);
		switch(ch)
		{
			case'i':top=push(top);
				break;
			case'd':top=pop(top);
				break;
			case't':peek(top);
				break;
			case'p':display(top);
				break;
			case'e':exit(0);
		}
	}
}
