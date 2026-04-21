#include<stdio.h>
#include<string.h>
char* mystrstr(char *,char *);
void main()
{
	char m[20],s[20],*res;
	printf("enter the main string and substring..\n");
	scanf("%s %s",m,s);
	res=mystrstr(m,s);
	if(res)
		printf("present..\n");
	else
		printf("not present..\n");

}
char* mystrstr(char *m,char *s)
{
	int i,j;
	for(i=0;m[i];i++)
	{
		for(j=0;s[j];j++)
		{
			if(m[i+j]!=s[j])
				break;
		}
		if(s[j]=='\0')
			return &s[j];
	}
	return NULL;
}
