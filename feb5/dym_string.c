#include<stdio.h>
#include<stdlib.h>

char* input_string(void)
{
	int i=0;
	static char *str=NULL;
	do
	{
		str=realloc(str,1);
		str[i]=getchar();
	}while(str[i++]!='\n');
	str[i]='\0';
	return str;
}

int main()
{
	char *str=NULL;
	str=input_string();
	printf("string : %s\n",str);
}
