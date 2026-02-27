#include<stdio.h>
#include<string.h>
char *mystrstr(const char *str,const char *sub)
{
	char *hay=(char *)str,*h=NULL;
	char *need=(char *)sub,*n=NULL;
	while(*hay)
	{
		h=hay;
		n=need;
		while(*h && *n)
		{
			if(*h==*n)
			{
				h++;
				n++;
			}
			else
				break;
		}
		if(*n=='\0')
			return hay;
		hay++;

	}
	return NULL;
}
int main()
{
	char str[30],sub[20],*p=NULL;
	fgets(str,30,stdin);if(str[strlen(str)-1]==10){str[strlen(str)-1]=0;}
	fgets(sub,20,stdin);if(sub[strlen(sub)-1]==10){sub[strlen(sub)-1]=0;}
	/*p=mystrstr(str,sub);
	if(p==NULL)*/
	p=strstr(str,sub);
	if(p==NULL){printf("no sub sub string\n");return 0;}
	while(p){
		printf("occured at %d\n",p-str);
	p=p+strlen(sub);
	p=strstr(p,sub);
	}
return 0;
}
