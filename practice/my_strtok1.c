#include<stdio.h>
#include<string.h>
char *mystrtok(char *src,const char *delim)
{
	if(src==NULL){return NULL;}
	static char *t=NULL;
	t=src;
	char *s=src;
	char *d=(char *)delim;
		while(*d && *t)
		{
			if(*t==*d)
			{
				s=t;
				t++;
				*s='\0';
				printf("%s\n",src);
				return s;
			}
			t++;
		}
		
			if(*t=='\0'){
				return NULL;
			}
		
	
	

}
int main()
{
	char str[20],deli[3],*p=NULL;
	strcpy(str,"hi hello hi");strcpy(deli," ");
	p=mystrtok(str,deli);
	while(p)
	{
		printf("%s\n",p);
		p=mystrtok(NULL,deli);
	}
	return 0;
}
