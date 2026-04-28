#include<stdio.h>
#include<string.h>

int main()
{
	char ch[100];
	printf("enter the string:");
	scanf("%[^\n]s",ch);
	for(int i=0;ch[i] && ch[i+1];i++)
	{
		if(ch[i]==' ' && ch[i+1]==' ')
		{
			memmove(ch+i,ch+i+1,strlen(ch+i+1));
					i--;
		}
	}
	printf("updated string is: %s\n",ch);

}
