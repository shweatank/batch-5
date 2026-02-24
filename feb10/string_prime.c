#include<stdio.h>
#include<string.h>
#include<math.h>

int check_prime(int num)
{
	int i,sq=sqrt(num);
	if(num<2)
		return 0;
	for(i=2;i<=sq;i++)
	{
		if(num%i==0)
			return 0;
	}
	return 1;
}

int check_setbit_cnt(int num)
{
	int i=0,cnt=0;
	for(i=0;i<32;i++)
	{
		if((num>>i)&1)
			cnt++;
	}
	if(check_prime(cnt))
		return 1;
	return 0;
}

void check_string(char *str)
{
	int arr[10];
	printf("string : %s\n",str);
	int i=0,j=0,len=strlen(str);
	for(i=0;i<len;i++)
	{
		if(check_prime(i))
		{
			if(check_setbit_cnt(str[i]))
			{
				arr[j++]=i;
			}
		}
	}
	for(i=0;i<j;i++)
	{
		memmove(str+arr[i]-i,str+arr[i]-i+1,len-(arr[i]-i)+1);
	}
	printf("string : %s\n",str);
}

int main()
{
	char str[100];
	printf("enter string  : ");
	scanf("%[^\n]s",str);

	check_string(str);
	printf("string : %s\n",str);
}
