#include<stdio.h>
#include<string.h>
int main()
{
	char str[32],comp[32],carry='1';
	printf("enter binary:");scanf("%s",str);
	int l=strlen(str);
	for(int i=0;i<l;i++)
	{
		if(str[i]=='0'){comp[i]='1';}
		else{comp[i]='0';}
	}
	puts(comp);
	if(comp[l-1]=='0'){comp[l-1]='1';}
	else
	{
		comp[l-1]='0';
		for(int i=l-2;i>=0;i--)
		{
			if(comp[i]=='0')
			{
				comp[i]=carry;break;
			}
			else
			{
				comp[i]='0';
			}

		}
	}
	puts(comp);

}
