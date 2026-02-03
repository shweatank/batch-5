#include<stdio.h>
#include<string.h>
void Check(char *,char *);
int main()
{
	char str1[100],str2[100];
	puts("enter the string 1");
	scanf("%s",str1);
	puts("enter the string 2");
	scanf("%s",str2);
	Check(str1,str2);
}
void Check(char *str1,char *str2)
{
	int i=0;
	char *c;
	while(str2[i])
	{
		if(strchr(str1,str2[i])==NULL)
			break;
		i++;
	}
	if(str2[i]!='\0')
		printf("%c\n",str2[i]);
}
