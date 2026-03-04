#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void CntFreq(char *);
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
int main()
{
	char *p=NULL;
	puts("enter the string");
	p=getstring();
	printf("entered string is %s\n",p);
	CntFreq(p);
}
void CntFreq(char *p)
{
	short i=0;
	int cnt=0,f;
	while(p[i])
	{
		char *t=p+i;
		cnt=0;
		f=0;
		for(int j=0;j<i;j++)
		{
			if(p[j]==p[i])
			{
				f=1;
				break;
			}
		}
		if(f==1)
		{
			i++;
			continue;
		}
		while(t=strchr(t,p[i]))
		{
			t++;
			cnt++;
		}
			printf("%c is %d\n",p[i],cnt);
		i++;
	}
}
