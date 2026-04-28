#include<stdio.h>
#include<string.h>
int main()
{
	char str[30],*p;
	printf("enter the string:");
	scanf("%s",str);
	for(int i=0;str[i];i++)
	{
		p=strchr(str+i+1,str[i]);
		if(p!=NULL)
		{
			memmove(p,p+1,strlen(p)+1);
			i--;
		}
	}
	printf("str is:%s\n",str);
}
