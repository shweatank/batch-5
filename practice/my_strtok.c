#include<stdio.h>
#include<string.h>
char *mystrtok(char *src,const char *delim)
{
	static char *p=NULL;
	if(src!=NULL)
	{
	p=src;
	}
	if(p==NULL || *p=='\0')
		return NULL;
	char *s=p;
	char *d=(char *)delim;
	while(*p && (*p!=*d))
		p++;
	if(*p==*d)
	{
		*p='\0';
	}
	else
	{
		p=NULL;
	}
	return s;

}
int main()
{
	char str[20],deli[3],*p=NULL;
	strcpy(str,"hi hello hi");strcpy(deli," ");
	p=mystrtok(str,deli);
	while(p)
	{
		printf("%s\n",p);
		p=mystrtok(NULL,deli);
	}
	return 0;
}
