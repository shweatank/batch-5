//counting frequency of each character in a given string


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char *getstring(void);
void Frequency(char *);
int main()
{
	char *s=NULL;
	puts("enter the string");
	s=getstring();
	Frequency(s);
}
char *getstring(void)
{
	int i=0;
	char *p=NULL;
	do
	{
		p=realloc(p,i+1);
		p[i]=getchar();

	}while(p[i++]!='\n');
	p[i-1]='\0';
	return p;
}
void Frequency(char *str)
{
	int cnt=0,i=0;
	char *p=NULL;
	char ch;
	for(;i<strlen(str);i++)
	{
		cnt=0;
		ch=str[i];
		for(int j=i;j<strlen(str);j++)
		{
			if(ch==str[j])
				cnt++;
		}
		printf("%c is freq is %d\n",ch,cnt);
	}
}
