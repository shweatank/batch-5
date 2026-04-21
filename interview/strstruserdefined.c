#include<stdio.h>
#include<string.h>
char* mystrstr(char *,char *);

void main()
{
	char s[20],s1[20];
	char *res;
	printf("enter the main string..\n");
	scanf("%s",s);
	printf("enter the sub string..\n");
	scanf("%s",s1);
	res=mystrstr(s,s1);
     if(res)
        printf("found at position: %ld\n", res - s);
    else
        printf("not found\n");
}

char* mystrstr(char *s, char *s1)
{
    int i, j;

    for(i = 0; s[i]; i++)
    {
	    
        for(j =0; s1[j]; j++)
        {
            if(s[i + j] != s1[j])
                break;
        }

        if(s1[j] == '\0')
            return &s[i];  
    }
    

    return NULL;  
}

char *my_strstr(char *s,char *d)
{

	   int i,j,k;

	   for(i=0;s[i];i++)
	   {
		   if(s[i]==d[0])
		   {
			      for(j=1,k=i+1;s[k]&&d[j];k++,j++)
			      {
				       if(s[k]!=d[j])
					       break;
			      }

			      if(d[j]=='\0')
				      return &s[i];

		   }
	   }

	   return NULL;
}















