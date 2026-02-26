#include<stdio.h>
#include<string.h>
#include<stdio_ext.h>
char* userdef_strstr(char *str,char *substr)
{
	int i,j,len=strlen(substr);
	for(i=0;str[i];i++)
	{
		if(str[i]==substr[0])
		{
			j=1;
			while(str[i+j]==substr[j++]);
			if(j==len)
				return str+i;
		}
	}
	return NULL;
}

int main()
{
	char *addr=NULL,str[100],substr[100];
	printf("enter string : ");
	scanf("%[^\n]s",str);
	__fpurge(stdin);
	printf("enter substring : ");
	scanf("%[^\n]s",substr);
	addr=userdef_strstr(str,substr);
	if(addr!=NULL)
		printf("%s not found in %s\n",substr,str);
	else
		printf("%s found in %s\n",substr,str);
	return 0;
}
