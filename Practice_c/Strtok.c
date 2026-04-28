#include<stdio.h>
#include<string.h>
char *str_tok(char *p,char *q)
{
	static char *temp=NULL;
	static int len1=-1;
	if(len1==-1)
		len1=strlen(p);
	else
		len1-=strlen(temp);
	if(len1<=0)
		return NULL;
	if(p==NULL)
		p=temp+strlen(temp)+1;
	temp=p;
	while(*p!=*q && *p)
		p++;
	if(*p==*q || *p=='\0')
	{	*p='\0';
		len1--;
	        return temp;
	}
	else
		return NULL;

}

int main()
{
	char str[60],*p;
	printf("enter the string:");
	scanf("%[^\n]s",str);
	p=str;
	while(p=str_tok(p," "))
	{
		printf("%s\n",p);
	        p=NULL;	
	}
}
