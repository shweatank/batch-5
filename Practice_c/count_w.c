#include<stdio.h>
#include<string.h>
int main()
{
	int nl=0,nc=0,nw=0;
	char ch;
	ch=getchar();
	while(ch!=EOF)
	{
		if(ch=='\n')
		{
			nl++;
			nw++;
		}
		else if(ch==' ') 
			nw++;
		else
			nc++;
		ch=getchar();
	}
	printf("nl=%d nc=%d nw=%d\n",nl,nc,nw); 
}
