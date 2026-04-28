#include<stdio.h>
#include<string.h>

void expand(char *s1,char *s2)
{
	int i=0,j=0;
	char c;
	for(i=0;s1[i];i++)
	{
	     if((s1[i]=='-') && (i>0) && (s1[i+1]!='\0') && (s1[i-1]<s1[i+1]))
		{
			for(c=s1[i-1]+1;c<=s1[i+1];c++)
				s2[j++]=c;
			i++;
		}
	     else
		     s2[j++]=s1[i];
	}
	s2[j]='\0';
}
int main()
{
	char s1[100],s2[100];
	printf("enter the string:");
	scanf("%s",s1);
	expand(s1,s2);
	printf("extended string is :%s\n",s2);
}
