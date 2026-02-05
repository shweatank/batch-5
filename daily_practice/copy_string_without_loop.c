#include<stdio.h>
#include<string.h>
void Copy(char *,char *,int );
int main()
{
	char str[100];
	puts("enter the string");
	fgets(str,100,stdin);
	int n=strlen(str);
	str[n-1]='\0';
	char dst[100];
	n=n-2;
	Copy(dst,str,n);
	printf("string 1 is %s\n",str);
	printf("copied string is %s\n",dst);
}
void Copy(char *dst,char *src,int n)
{
	static int k=0;
	if(k>n)
	{
		dst[k]='\0';
		return;
	}
	else
	{
		dst[k]=src[k];
		k++;
		Copy(dst,src,n);
	}
}
