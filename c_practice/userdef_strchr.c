#include<stdio.h>
#include<stdio_ext.h>
#include<string.h>
char* userdef_strchr(char *str,char ch)
{
	int i=0;
	for(i=0;i<strlen(str);i++)
	{
		if(str[i]==ch)
			return str+i;
	}
	return NULL;
}

int main()
{
	char str[100],ch,*addr=NULL;
	printf("enter string : ");
	scanf("%s",str);
	__fpurge(stdin);
	printf("enter char to be searched : ");
	scanf("%c",&ch);
	addr=userdef_strchr(str,ch);
	if(addr)
		printf("%c found at %ld location\n",ch,addr-str);
	else
		printf("%c not found in %s \n",ch,str);
	return 0;
}
