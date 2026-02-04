//userdef memmove

#include<stdio.h>


void userdef_memmove(char *dest,char*src,int nbyt)
{
	for(int i=0;i<nbyt;i++)
	{
		dest[i]=src[i];
	}
}

int main()
{
	char str[20];
	printf("enter string :  ");
	scanf("%s",str);
	printf("before memmove : %s\n",str);
	
	userdef_memmove(str+2,str+4,2);

	printf("after memmove : %s\n",str);
}
