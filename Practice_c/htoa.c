#include<stdio.h>
#include<string.h>


int htoi(char *p)
{
	int num=0,num1=0,j=0;
	char temp;
	for(int i=2,j=strlen(p)-1;i<j;i++,j--)
	{
		temp=p[i];
		p[i]=p[j];
		p[j]=temp;
	}
	for(int i=2;p[i];i++)
	{
		num1=(p[i]-48);
		num|=(num1<<((j++)*4));
	}
	return num;
}
/*int check_hexa(char *str)
{
	for(i=0;str[i];i++)
}*/
int main()
{
	int num;
	char str[10];
	printf("enter the string:");scanf("%s",str);
/*	if(check_hexa(str)==0)
	{
		printf("not a valid hexa decimal\n");
		return 0;
	}*/
	num=htoi(str);
	printf("number is %d\n",num);
}
