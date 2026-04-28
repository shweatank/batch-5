#include<stdio.h>
#include<string.h>

char find_char(char *s1,cha
{
	while(s2)
	{
		if(!strchr(s1,*s2))
		return *s2;
	s2++;
	}
return -1;	
}

int main()
{
	char s1[100],s2[100],ch;
	printf("enter string1:");
	scanf("%[^\n]s",s1);
	printf("enter string2:");scanf("%s",s2);
	ch=find_char(s1,s2);
	printf("char is %c\n",ch);
}

