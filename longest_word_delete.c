#include<stdio.h>
#include<string.h>

void main()
{
	char s[100],r[50]="";

	printf("enter the string:\n");
	scanf("%[^\n]",s);


      int len=0,prev_len=0;

      char *p,*q;

      int i,j;


      int flag=1,l=0,flag1=0;

	for(i=0;s[i];i++)
	{

		if(flag)
		{
			while(s[l]!=' '&&s[l]!='\0')
				l++;

			prev_len=l;

			p=&s[i];
			q=&s[l];

			flag=0;
			flag1=1;

			i=l;
		}
		
		for(j=i,len=0;s[j]!=' '&&s[j]!='\0';j++,len++);

		if(len>prev_len)
		{
			flag1=0;
			prev_len=len;
			p=&s[i];
			q=&s[j];
		}

		i=j;
	}

    if(flag1)
    {
	   strcat(r,s+l+1);
	   strcpy(s,r);
    }
    else
    {

       p[-1]='\0';

       strcat(s,q);
    }

       printf("%s",s);


}
