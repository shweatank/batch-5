#include<stdio.h>

int main()
{
	char ch;
	int len=0,word=0;
	ch=getchar();
	while(ch!=EOF)
	{
		if(ch==' ' || ch=='\t' || ch=='\n')
		{
			printf(" :lenght %d \n",len);
			len=0;
		}
		else
		{
			len++;
			putchar(ch);
		}
		ch=getchar();
	}
}
