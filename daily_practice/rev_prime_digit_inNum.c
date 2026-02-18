#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
int prime(int );
int Rev(int );
void Print(int *,int );
void Input(int *arr,int size)
{
	srand(time(0));
	for(int i=0;i<size;i++)
	{
		arr[i]=rand()%50000+1;
	}
}
void Revprime(int *arr,int size)
{
	int num=0,n;
	char str[20];
	int i=0;
	for(int k=0;k<size;k++)
	{
		i=0;
		num=0;
		n=arr[k];
		while(n)
		{
			str[i++]=((n%10)+48);
			n/=10;
		}
		str[i]='\0';
		for(int i=0,j=strlen(str)-1;i<j;i++,j--)
		{
			if(prime(str[i]-48))
			{
				while(!prime(str[j]-48))
					j--;
				char ch=str[i];
				str[i]=str[j];
				str[j]=ch;
			}
		}
		for(i=strlen(str)-1;i>=0;i--)
		{
			num=(num*10)+(str[i]-48);
		}
		memset(str,0,sizeof(str));
		printf("%d ",num);
	}
	puts("");
}
void Print(int *arr,int s)
{
	for(int i=0;i<s;i++)
	{
		printf("%d ",arr[i]);
	}
	puts("");
}

int prime(int n)
{
	int s=sqrt(n);
	if(n<2)
		return 0;
	if(n==2)
		return 1;
	for(int i=2;i<=s;i++)
	{
		if(n%i==0)
			return 0;
	}
	return 1;
}
int main()
{
	int arr[15];
	Input(arr,15);
	Print(arr,15);
	Revprime(arr,15);
}
