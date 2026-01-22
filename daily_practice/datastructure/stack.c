#include<stdio.h>
#include<stdlib.h>
#include<stdio_ext.h>


#define MAX 10

int top=-1;

void display(int *p)
{
	if(top==-1)
	{
		puts("no elements in stack");
		return;
	}
	int i=0;
	puts("elements in stack");
	for(;i<=top;i++)
	{
		printf("%d ",p[i]);
	}
	printf("\n");
}

void push(int *p)
{
	int n;
	if(top!=MAX-1)
	{
		puts("enter the element to push");
		scanf("%d",&n);
		p[++top]=n;
	}
	else
	{
		puts("stack is overflow");
	}

}

void pop(int *p)
{
	if(top==-1)
	{
		puts("stack is underflow");
		return ;
	}
	top--;
}
void peek(int *p)
{
	if(top!=-1)
	{
		puts("peek elements is");
		printf("%d\n",p[top]);
	}
}

int main()
{
	int *p=NULL;
	p=malloc(MAX*sizeof(int));
	if(p==NULL)
	{
		puts("memory is not allocated");
		return 0;
	}
	char ch;
	while(1)
	{
		__fpurge(stdin);
		puts("i : push d: pop t: peek p: display e: exit");
		scanf("%c",&ch);
		switch(ch)
		{
			case'i':push(p);
				break;
			case'd':pop(p);
				break;
			case'p':display(p);
				break;
			case't':peek(p);
				break;
			case'e':exit(0);
		}

	}
}
