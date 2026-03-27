#include<stdio.h>
#include<string.h>
void main()
{
	char s[]="d t se rch";

	int len=strlen(s),i;
	for(i=0;i<len;i++)
	{
		if(s[i]==' ')
			s[i]='a';
	}

	printf("resultant=%s\n",s);
}


