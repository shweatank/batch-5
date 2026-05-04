#include<stdio.h>
int anagram(const char *, char *);
void main()
{
	char s1[20];
	char s2[20];
	printf("enter the string s1\n");
	scanf("%[^\n]",s1);

	printf("enter the string s2\n");
	scanf(" %[^\n]",s2);

	int len1,len2;
	for(len1=0;s1[len1];len1++);
	for(len2=0;s1[len2];len2++);
	int r;
	if(len1==len2)
	{
		r=anagram(s1,s2);
		printf("%d\n",r);
		if(r==1)
			printf("its anagram\n");
		else
			printf("not anagram\n");
	}
	else
		printf("strings are not anagram\n");
}

int anagram(const char * s1, char *s2)
{
	int i,j,c;
	for(i=0;s1[i];i++)
	{
		for(j=0,c=0;s2[j];j++)
		{
			if(c==0)
			{
				if(s1[i]==s2[j])
 				{
					c++;
					s2[j]=s2[j+1];
					j--;
				}
			}
		
		}
if(c==0)
return 0;

	}
return 1;
}

