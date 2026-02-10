#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
char *getstring(void)
{
	int i=0;
	char *p=NULL;
	do
	{
		p=realloc(p,(i+1)*sizeof(char));
		p[i]=getchar();
	}while(p[i++]!='\n');
	p[i-1]='\0';
	return p;
}
int Prime(int n)
{
	if(n<2)
		return 0;
	if(n==2)
		return 1;
	int s=sqrt(n);
	int i;
	for(i=2;i<=s;i++)
	{
		if(n%i==0)
			return 0;
	}
	return 1;
}
int SetBit(int n)
{
	int cnt=0;
	while(n)
	{
		n=n&n-1;
		cnt++;
	}
	return cnt;
}
void DelChar(char *str)
{
	int n=strlen(str);
	int i,j=0;
	int setcnt;
	int arr[100]={};
	setcnt=SetBit(str[2]);
	if(Prime(setcnt))
	{
		arr[j]=2;
		j++;
	}
	for(i=3;i<n;i++)
	{
		if(Prime(i))
		{
			setcnt=SetBit(str[i]);
			if(Prime(setcnt))
			{
				arr[j]=i;
				j++;
			}
		}
	}
	for(i=0;i<100;i++)
	{
		if(arr[i]==0)
			break;
			memmove(str+(arr[i]-i),str+(arr[i]-i)+1,strlen(str+(arr[i]-i)+1)+1);
	}
	printf("%s\n",str);
}
int main()
{
	char *str=NULL;
	str=getstring();
	DelChar(str);
}
