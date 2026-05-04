#include<stdio.h>
#include<string.h>
void main()
{
char s[100],ow[20],nw[20],*p,*q,t[100];
printf("enter the str\n");
scanf("%[^\n]",s);
printf("enter the original word\n");
scanf("%s",ow);
printf("enter the new word\n");
scanf("%s",nw);
p=s;
printf("before:%s\n",s);
q=strstr(p,s);
strcpy(t,q+strlen(ow));
strcpy(q,nw);
strcat(q,t);
printf("%s\n",s);
}
