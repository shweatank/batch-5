#include<stdio.h>
#include<stdlib.h>


int any(const char *s1,const char *s2)
{
	int i,j;
	for(i=0;s1[i];i++)
	{
		for(j=0;s2[j];j++)
		{
			if(s1[i]==s2[j])
				return i;
		}
	}
	return -1;

}
int main()
{
	char s1[100],s2[100];
	printf("enter the string1:");scanf("%s",s1);
	printf("enter the string1:");scanf("%s",s2);
	printf("position:%d\n",any(s1,s2));
}
