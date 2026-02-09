#include<stdio.h>
#include<math.h>
void data(char *,int *);
void primecheck(int*,int *);
int main()
{
	int n=0;
	char ch;
	data(&ch,&n);
}
void data(char *ch,int *n)
{
	int x;
	char *ptr=(char *)n;
	for(int k=0;k<sizeof(*n);k++)
	{
		puts("enter the char number");
		scanf(" %d",ch);
		printf("%d\n",*ch);
		*ptr=*ch;
		if(k==0 || k==3)
		{
			x=x+*ch;
		}
		ptr++;
	}
	int i;
	printf("%d\n",*n);
	printf("%d\n",x);
	primecheck(&i,&x);
	if(i==0)
	{
		puts("not prime");
	}
	else if(i==1)
	{
		puts("prime");
	}
}
void primecheck(int*i,int *x)
{
	if(*x<2)
	{
		*i=0;
		return;
	}
	if(*x==2)
	{
		*i=1;
		return;
	}
	int s=sqrt(*x);
	for(int j=2;j<=s;j++)
	{
		if(*x%j==0)
		{
			*i=0;
			return;
		}
	}
	*i=1;
}
