#include<stdio.h>

int main()
{
	char ch;
	int Capfre[26]={0},SmaFre[26]={0},digitfre[10]={0},i,j,k;
	ch=getchar();
	while(ch!=getchar())
	{
		if(ch>='0' && ch<='9')
		{
			++digitfre[ch-'0'];
		}
		else if(ch>='A' && ch<='Z')
			++Capfre[ch-'A'];
		else if(ch >='a' && ch<='z')
			++SmaFre[ch-'a'];
		else if(ch==' ' || ch=='\t' || ch=='\n')
		{
			for(i=0,k=0;i<26;i++)
			       
		}
}
