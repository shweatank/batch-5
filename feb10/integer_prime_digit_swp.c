#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>

int check_prime(int num)
{
	int i,sq;
	if(num<2)
		return 0;
	sq=sqrt(num);
	for(i=2;i<=sq;i++)
	{
		if(num%i==0)
			return 0;
	}
	return 1;
}

void reverse_str(char* str)
{
	int i,j;
	char ch;
	for(i=0,j=strlen(str)-1;i<j;i++,j--)
	{
		ch=str[i];
		str[i]=str[j];
		str[j]=ch;
	}
}

int main()
{
	int num;
	printf("enter number : ");
	scanf("%d",&num);
	int temp=num,i=0;
	char str[10],ch;
	while(temp)
	{
		str[i++]=(temp%10)+48;
		temp/=10;
	}

	str[i]='\0';
	reverse_str(str);

	int j=strlen(str)-1;
	for(int i=0;i<strlen(str);i++)
	{
		if(check_prime(str[i]-48))
		{
			while(check_prime(str[j]-48) && j!=i)
			{
				ch=str[i];
				str[i]=str[j];
				str[j]=ch;
				if(j==i)
					break;
				j--;
				continue;
			}
		}
	}

	num=atoi(str);
	printf("number : %d\n",num);
}
