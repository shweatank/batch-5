#include<stdio.h>
#include<string.h>

void escape(char *s,char *t)
{
	int i,j;
	for(i=0,j=0;s[i];i++)
	{
		if(s[i]=='\n')
		{
			t[j++]='\\';
			t[j++]='n';
		}
		else if(s[i]=='\t')
		{
			t[j++]='\\';
			t[j++]='t';
		}
		else
			t[j++]=s[i];
	}
	t[j]='\0';
}
int main()
{
	int i=0;
	char s[100],t[100];
	printf("enter the string:");
	do
	{
		s[i]=getchar();
	}while(s[i++]!=EOF);
	escape(s,t);
	printf("\nafter conversion %s\n",t);
}
