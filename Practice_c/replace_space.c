#include<stdio.h>
int main()
{
	char ch,prev=0;
	ch=getchar();
	while(ch!=EOF)
	{
		if(ch==' ')
		{
			if(prev==0)
			{
				putchar(ch);
				prev=1;
			}
		}
		else
		{
			putchar(ch);
			prev=0;
		}
		ch=getchar();
	}
}
