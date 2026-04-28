#include<stdio.h>
int main()
{
	char ch;
	ch=getchar();
	while(ch!=EOF)
	{
		if(ch=='\t')
		{
			printf("\\t ");
		}
		else if(ch=='\b')
			printf("\\b ");
		else if(ch=='\\')
			printf("\\ ");
		else
			putchar(ch);
		ch=getchar();
	}
}
