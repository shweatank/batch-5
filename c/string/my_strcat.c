#include<stdio.h>
char * my_strcat(char *d,const char *s); 
void main()
{
char s[10], d[20];
printf("enter the d string\n");
scanf("%s",d);
printf("enter the s string\n");
scanf("%s",s);
char *p=my_strcat(d,s);
printf("strcat sting=%s\n",p);
}

char * my_strcat(char *d,const char *s)
{
int i,len;
for(len=0; d[len]; len++);
printf("dest len=%d\n",len);

for(i=0;s[i]; i++)
d[len+i]=s[i];
d[len+i]=s[i];
return d;
}

