#include<stdio.h>
#include<stdlib.h>
int isvowel(char ch)
{
	if(ch=='a'||ch=='e'||ch=='i'||ch=='o'||ch=='u'||ch=='A'||ch=='I'||ch=='O'||ch=='U')
		return 1;
	else
		return 0;
}
void main()
{
	int i,j=0;
	char s[100];

	printf("enter the string:\n");
	scanf("%s",s);

	char *p[100];

	for(i=0;s[i];i++)
	{

               if(isvowel(s[i]))
		p[j++]=&s[i];
	}

	char *q=malloc(1);

        int l;

	for(i=0,l=j-1;i<l;i++,l--)
	{
		*q=*p[i];
		*p[i]=*p[l];
		*p[l]=*q;
	}

	printf("%s",s);

}
