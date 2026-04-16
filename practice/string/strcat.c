#include<stdio.h>
void main()
{
	char s[20],s1[20];
	int i,j;
	printf("enter the  two stringg...\n");
	scanf("%s %s",s,s1);
	//for(i=0;s[i];i++);
	i=strlen(s);
	for(j=0;s[i]=s1[j];j++,i++);
	
          printf("concatstring:%s",s);
}
/*///////predefined
#include<string.h>
void main()
{
	char s[20],s1[20];
	printf("enter the  two stringg...\n");
	scanf("%s %s",s,s1);
	strcat(s,s1);
          printf("concatstring:%s",s);
}
*/
