#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int i;

char *getstring(void)
{
	char *str=NULL;
	int i=0;
	do
	{
		str=realloc(str,(i+1)*sizeof(char));
		str[i]=getchar();
	}while(str[i++]!='\n');
	str[i-1]='\0';
	return str;
}

char *WordLength(char *str,int *hig)
{
	char *temp,*freq=NULL;
	int len;
	temp=strtok(str," ");
	len=strlen(temp);
	printf("word %s\t length %d\n",temp,len);
	freq=realloc(freq,i+1);
	freq[i++]=len+48;
	if(len>*hig)
		*hig=len;
	while(temp=strtok(NULL," "))
	{

		len=strlen(temp);
		printf("word %s\t length %d\n",temp,len);
		freq=realloc(freq,i+1);
		freq[i++]=len+48;
		if(len>*hig)
			*hig=len;
	}
	return freq;
}

void HorizontalFrequency(char *freq,int *hig)
{
	puts("horizontal");
	char *temp=NULL;
	int cnt=0;
	for(int j=1;j<=*hig;j++)
	{
		cnt=0;
		temp=freq;
		while(temp=strchr(temp,j+48))
		{
			cnt++;
			temp++;
		}
		printf("length %d\t",j);
		for(int m=0;m<cnt;m++)
			printf("* ");
		puts("");
	}
	
}

void VerticalFrequency(char *freq,int *hig)
{
	puts("vertical");
	char *temp=NULL;
	int cnt=0,b;
	char **ver;
	ver=calloc(1,((*hig)+1)*sizeof(char *));
	for(int m=0;m<=*hig;m++)
	{
		ver[m]=calloc(1,(*hig)*sizeof(char));
	}
	for(int j=0;j<*hig;j++)
	{
		temp=freq;
		cnt=0;
		while(temp=strchr(temp,j+48))
		{
			cnt++;
			temp++;
		}
		for(b=0;b<cnt;b++)
		{
			ver[b][j]='*';
		}
		ver[*hig][j]=(j+1)+48;
	}
	for(int a=0;a<=*hig;a++)
	{
		for(int b=0;b<*hig;b++)
		{
			printf("%c ",ver[a][b]);
		}
		puts("");
	}
	free(ver);
}

int main()
{
	char *str=NULL;
	char *freq=NULL;
	int hig=-1;
	str=getstring();
	freq=WordLength(str,&hig);
	HorizontalFrequency(freq,&hig);
	VerticalFrequency(freq,&hig);
	free(str);
	free(freq);
}
