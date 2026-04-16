#include<stdio.h>
#include<string.h>
void delete(char *s,char ch)
{
          
	    int i,j;

	    int len=strlen(s);

	    for(i=0;i<len;i++)
	    {
		    if(s[i]==ch)
		    {
			    for(j=i;j<len;j++)
				    s[j]=s[j+1];

			    i--;
		    }
	    }
}

int main()
{
	char s1[50],s2[50];


	printf("enter the strings s1,s2\n");

	scanf("%[^\n] %[^\n]",s1,s2);
	//printf("%s\n %s\n",s1,s2);

	int i,len=strlen(s2);
	for(i=0;i<len;i++)
	{
		delete(s1,s2[i]);
	}

	printf("resultant string=%s\n",s1);

	return 0;
}
