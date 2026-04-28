#include<stdio.h>
#include<string.h>

void mysqueeze(char *s1,const char *s2)
{
	int i,j,k=0,flag;
	for(i=0;s1[i];i++)
	{
		flag=0;
		for(j=0;s2[j];j++)
		{
			if(s1[i]==s2[j])
			{
				flag=1;
				break;
			}
		}
		if(!flag)
			s1[k++]=s1[i];
	}
	s1[k]='\0';
}

int main()
{
	char s1[100],s2[100];
	printf("enter the string1:");scanf("%s",s1);
	printf("enter the string1:");scanf("%s",s2);
	mysqueeze(s1,s2);
	printf("updated string:%s\n",s1);
}
