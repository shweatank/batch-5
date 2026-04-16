#include<stdio.h>
void main()
{
	char s[20],ch;
	int i,j,flag=0;
	printf("enter the  two stringg...\n");
	scanf("%s",s);
	printf("enter the char..\n");
	scanf(" %c",&ch);
	for(i=0;s[i];i++)
	{
		if(s[i]==ch)
		{
			flag=1;
		}
	}
	if(flag)
		printf("found\n");
	else
	        printf("Not found\n");
}
/*///////predefined
#include<string.h>
void main()
{
	char s[20],*p,ch;
	printf("enter the  two stringg...\n");
	scanf("%s",s);
	printf("enter char.\n");
	scanf(" %c",&ch);
	p=strchr(s,ch);
	if(p)
	printf("found");
	else
	printf("not found");
	
}
*/
