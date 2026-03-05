#include<stdio.h>
char * my_strcpy(char *d, const char *s);
void main()
{
char s[10],d[20];
int i;
char *p;
printf("enter the src string\n");
scanf("%s",s);
p=my_strcpy(d,s);
printf("destn str: %s\n",p);

}

char * my_strcpy(char *d, const char *s)
{
int i;
for(i=0; s[i]; i++)
d[i]=s[i];
d[i]=s[i];

return d;
}
