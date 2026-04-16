#include<stdio.h>
#include<string.h>
void main()
{
	char s[30],*p,*q,temp;
	printf("enter the string..\n");
	scanf("%s",s);
	p=s;
	q=s+strlen(s)-1;
	while(p<q)
	{
		temp=*p;
		*p=*q;
		*q=temp;
		p++;
		q--;

	}
	printf("revstring:%s\n",s);
}
/*void main()
{
	char s[40],temp;
	int i,j,len;
	printf("enter the string...........\n");
	scanf("%s",s);
	len=strlen(s);
	for(i=0,j=len-1;i<j;i++,j--)
	{
		temp=s[i];
		s[i]=s[j];
		s[j]=temp;
	}
	printf("rev string:%s",s);
}
*/
