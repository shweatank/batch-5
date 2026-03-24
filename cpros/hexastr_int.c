#include"header.h"
void main()
{
	int n=0,i,l,pos;
	char s[10]="0x123",ch;
	printf("enter the string in hexa format\n");
	scanf("%s",s);
	if((s[0]=='0'&& s[1]=='X') || (s[0]=='0' && s[1]=='x'))
	{
		l=strlen(s)-1;
		for(pos=0;l>=2;l--,pos+=4)
		{
			if(s[l]>='0'&& s[l]<='9')
			{
				ch=s[l]-'0';
				n|=ch<<pos;
			}
			else if(s[l]>='A'&& s[l]<='F')
			{
				ch=s[l]-55;
				n|=(ch<<pos);
			}
			else if(s[l]>='a'&& s[l]<='f')
			{
				ch=s[l]-87;
				n|=(ch<<pos);
			}
			else
			{
				printf("invlaid string\n");
				return;
			}
		}
	}
	else
	{
		printf("string not in hexaformat\n");
		return;
	}
	printf("%d\n",n);
}
