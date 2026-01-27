#include<stdio.h>
#include<string.h>
void Remove(char *,char *);
int main()
{
	char str[100];
	puts("enter the string");
	fgets(str,100,stdin);
	char substr[50];
	puts("enter the sub string");
	scanf("%s",substr);
	str[strlen(str)-1]='\0';
	Remove(str,substr);
	printf("removed string is %s\n",str);
}
void Remove(char *str,char *substr)
{
	char *p=NULL;
	if(p=strstr(str,substr))
	{
		memmove(p,p+strlen(substr),strlen(p+strlen(substr))+1);
	}
}


