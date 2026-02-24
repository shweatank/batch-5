#include<stdio.h>
#include<string.h>
int max;

int check_palindrome(char *str,int c)
{
	//printf("%d\n",c);
	int i,j;
	for(i=0,j=c-1;i<j;i++,j--)
	{
		if(str[i]!=str[j])
			return 0;
	}
	return 1;
}

char* find_palindrome(char *str)
{
	int i=0,j;
	int cnt=0;
	char *ptr=NULL;
	while(str[i])
	{
		for(j=i;j<=strlen(str);j++)
		{
			cnt=0;
			if(check_palindrome(str+i,j-i))
			{
				cnt=j-i;
				if(cnt>max)
				{
					max=cnt;
					ptr=str+i;
				}
			}
			//printf("cnt :%d\n",cnt);
		}
		i++;
	}
	return ptr;
}

int main()
{
	char str[20];
	scanf("%[^\n]s",str);
	char *ptr=find_palindrome(str);
	int i=0;
	while(i<max)
		printf("%c",ptr[i++]);
}
