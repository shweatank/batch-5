#include<stdio.h>
void main()
{
	char s1[20],s2[30];
	int i,j,k=0,c;
	printf("enter the two strings..\n");
	scanf("%s%s",s1,s2);
	for(i=0,c=0;s1[i];i++)
	{
		for(j=0;s2[j];j++)
	     {

		if(s1[i]==s2[j])
		{
			
			for(k=i;s1[k];k++)
				s1[k]=s1[k+1];
			   i--;
		}
	
	     }

	}
	
	printf("%s\n",s1);
}
