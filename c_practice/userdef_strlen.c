#include<stdio.h>
int userdef_strlen(char *str)
{
	int i=0;
	while(*str++)i++;
	return i;
}

int main()
{
	char str[100];
	printf("enter string : ");
	scanf("%s",str);
	printf("string length : %d\n",userdef_strlen(str));
}
