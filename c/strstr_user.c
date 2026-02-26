#include<stdio.h>
void main()
{
	char s[100],m[100];
	int i,j,found=0;
	printf("Enter the main string:\n");
	scanf("%s",m);
	printf("Enter the substring:\n");
	scanf("%s",s);
	for(i=0;m[i];i++)
	{
		j=0;
		while(m[i+j] == s[j] && s[j]!='\0')
		{
			j++;
		}
		if(s[j] == '\0')
		{
			found=1;
			break;
		}
	}
		if(found)
			printf("Substring exists\n");
		else
			printf("Substring not exists\n");
}
