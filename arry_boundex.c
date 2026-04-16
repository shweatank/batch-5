#include<string.h>
#include<stdio.h>
#include<stdlib.h>
void main()
{
	char *s=malloc(1024),s1[10];

	printf("enter the document in the form of words:\n");

         scanf("%[^\n]",s);

	 int len;
        while((len=sscanf(s,"%s",s1))!=-1)
	{
		printf("%s\n",s1);
		s=s+strlen(s1)+1;
	}



}
