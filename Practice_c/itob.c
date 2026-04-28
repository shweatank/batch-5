#include<stdio.h>

void itob(char *s,int n)
{
	char ch;
	int i=0,j;
	while(n)
	{
		s[i++]=n%2+48;
		n/=2;
	}
	s[i]='\0';
	
	printf("string:%s\n",s);
	for(j=0,i-=1;j<i;i--,j++)
	{
		ch=s[i];
		s[i]=s[j];
		s[j]=ch;
	}
	printf("string:%s\n",s);
}
int main()
{
	int num;
	char s[100];
	printf("enter the num:");
	scanf("%d",&num);
	itob(s,num);
	printf("the binary string is:%s\n",s);
}
