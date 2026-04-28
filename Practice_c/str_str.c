#include<stdio.h>

char *my_strstr(char *s,char *sub)
{
	int flag=0;
	char *s1,*s2,*temp;
	s1=s;
	while(*s1)
	{
		s2=sub;
		temp=s1;
		while(*s1 && *s2 && (*s1==*s2))
		{
			s1++;
			s2++;
		}
		if(s2==NULL)
			return temp;
		s1=temp+1;
	}

	return NULL;
}
int main()
{
	char s1[50],s2[10],*p;
	printf("enter the string1:");scanf("%[^\n]s",s1);
	printf("enter the string2:");scanf("%s",s2);
	p=my_strstr(s1,s2);
	if(p)
	printf("%s\n",p);
	else
		printf("not found\n");
}
