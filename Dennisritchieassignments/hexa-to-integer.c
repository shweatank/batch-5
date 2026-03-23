#include<stdio.h>
void main()
{
	char s[30];
	int i,res=0,d;
	scanf("%s",s);
	printf("%s",s);
	for(i=0;s[i];i++)
	{
           if(s[i]>='0' && s[i]<='9')
	 	   d=s[i]-'0';
            else if(s[i]>='a' && s[i]<='f')
		    d=s[i]-'a'+10;
	   else if(s[i]>='A' && s[i]<='F')
		   d=s[i]-'A'+10;
	   else
		   printf("Invalid argument\n");
	
	res=res*16+d;
	}
	printf("final :%d",res);
}
	   
