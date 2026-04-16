#include<stdio.h>
int main()
{
	char s[20],s1[20];
	int i;
	printf("enter the  two stringg...\n");
	scanf("%s %s",s,s1);
	for(i=0;s[i]&&s1[i];i++)
	{	
		if(s[i]!=s1[i])
		{
		
			printf("not equal\n");
			return 0;
		}
	}
	if(s[i]==s1[i])
		printf(" equal\n");
	else 
		printf("not equal\n");
	return 0;

	
}
/* ///////predefined
#include<string.h>
void main()
{
	char s[20],s1[20];
	printf("enter the  two stringg...\n");
	scanf("%s %s",s,s1);
	if(strcmp(s,s1)==0)
		printf("equal\n");
	else
		printf("not equal\n");
}
*/
