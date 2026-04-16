#include<stdio.h>
#include<stdlib.h>
#define MAX 100


typedef struct {
	char data[MAX];

	int top;
}stack;


void init(stack *s)
{
	s->top=-1;
}

void push(stack *s,char c)
{
	if(s->top>=MAX)
	{
		printf("stack is overflow:\n");
		return;
	}

	s->data[++(s->top)]=c;
}

char  pop(stack *s)
{
	if(s->top==-1)
	{
		printf("stack is empty :\n");
			return -1;
	}

	return s->data[(s->top)--];
}
int ismatched(char o,char c)
{
	if(o=='{' && c=='}')
		return 1;
	if(o=='[' && c==']')
		return 1;
	if(o=='(' && c==')')
		return 1;

	return 0;
}
int isbalanced(char *str)
{
	stack s;
	init(&s);


	int i=0;


	for(i=0;str[i]!='\0';i++)
	{
		if(str[i]=='{'||str[i]=='['||str[i]=='(')
		{
			push(&s,str[i]);
		}
		else if(str[i]=='}'||str[i]==']'||str[i]==')')
		{
			char c=pop(&s);


		//	printf("%c %c\n",c,str[i]);

			if(!ismatched(c,str[i]))
				return 0;
		}



	}


      return s.top == -1;
}
int main()
{
	char s[]="{[[[[[[[{[]}]]]]]]}";

	if(isbalanced(s))
	{
		printf("balnaced");
	}
	else
	{
		printf("not balnaced");
	}
}

