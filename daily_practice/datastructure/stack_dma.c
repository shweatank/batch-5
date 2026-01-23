#include<stdio.h>
#include<stdio_ext.h>
#include<stdlib.h>
struct stack
{
	int *arr;
	int top;
	int size;
};
struct stack push(struct stack *ptr)
{
	int n;
	if(ptr->top!=ptr->size-1)
	{
		++ptr->top;
		ptr->arr=realloc(ptr->arr,(ptr->top+1)*sizeof(int));
		printf("enter the element\n");
		scanf("%d",&n);
		ptr->arr[ptr->top]=n;
	}
	else
	{
		puts("stack is overflow");
	}
	return *ptr;
}
void display(struct stack var)
{
	int i=0;
	for(;i<=var.top;i++)
	{
		printf("%d ",var.arr[i]);
	}
	printf("\n");

}

void peek(int *arr,int top)
{	
	if(top==-1)
		puts("no peek element\n");
	else
		printf("peek element is %d\n",arr[top]);
}
struct stack pop(struct stack *ptr)
{
	if(ptr->top==-1)
	{
		puts("stack is underflow\n");
	}
	else
	{
		ptr->top--;
	}
	return *ptr;
}
int main()
{
	struct stack var;
	puts("enter the size");
	scanf("%d",&var.size);
	char ch;
	var.top=-1;
	while(1)
	{
		__fpurge(stdin);
		puts("enter i:push d:pop t:peek p:print e:exit");
		scanf("%c",&ch);
		switch(ch)
		{
			case'i':var=push(&var);
				break;
			case'd':var=pop(&var);
				break;
			case't':peek(var.arr,var.top);
				break;
			case'p':display(var);
				break;
			case'e':exit(0);
		}
	}
}
