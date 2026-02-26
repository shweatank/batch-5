#include<stdio.h>

void userdef_strcpy(char *dest,char *src)
{
	int i=0;
	while(src[i])
	{
		dest[i]=src[i];
		i++;
	}
	dest[i]='\0';
}

int main()
{
	char src[100],dest[100];
	printf("enter src string : ");
	scanf("%s",src);
	userdef_strcpy(dest,src);
	printf("dest str : %s\n",dest);
	return 0;
}
