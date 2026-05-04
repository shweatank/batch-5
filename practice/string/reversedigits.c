#include<stdio.h>
#include<string.h>
void main()
{
	char s[30];
	int i,j,a,k,m,temp,len,b;
	printf("enter the string..\n");
	scanf("%[^\n]",s);
	len=strlen(s);
	for(i=0,a=0;i<=len;i++)
	{
		if(s[i]==' ' || s[i]=='\0')
		{
			j=i;
			for(b=a,k=j-1;b<=k;b++)
			{
				if(s[b]>='0' && s[b]<='9')
				{
					for(a,k;a<=k;a++,k--)
					{
						temp=s[a];
						s[a]=s[k];
						s[k]=temp;
					}
					
				}
			}
			a=i+1;
		}

	}
	printf("%s",s);
}
