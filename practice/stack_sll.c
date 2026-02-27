#include<stdio.h>
#include<stdlib.h>
typedef struct st
{
	int data;
	struct st *link;
}ST;
ST *top=NULL;
void push(int d)
{
	ST *nn=calloc(1,sizeof(nn));
	nn->data=d;
	if(top==NULL)
		top=nn;
	else{
		nn->link=top;
		top=nn;}

}
int pop()
{
	ST *temp=NULL;
	if(top==NULL)
		return -1;
	else
	{
		int d=top->data;
		temp=top;
		top=top->link;
		free(temp);
		temp=NULL;
		return d;
	}
}
int peek()
{
	if(top)
	return top->data;
	else
		return -1;
}
int main()
{
	int ch,dat;
	while(1)
	{
		printf("enter your choice:");scanf("%d",&ch);
		switch(ch)
		{
			case 1:printf("enetr data");scanf("%d",&dat);
			       push(dat);break;
			case 2:dat=pop();
			       if(dat==-1)
				       printf("stack empty\n");
			       else
				       printf("poped data:%d\n",dat);
			       break;
			case 3:dat=peek();
			       if(dat==-1)
				       printf("empty stack\n");
			       else
				       printf("top points to %d\n",dat);
			       break;
			case 4:return 0;
		}
	}
	return 0;
}
