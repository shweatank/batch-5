#include<stdio.h>
#include<string.h>
char *token;
char str[20]="sai kumar bye";
char* strtok1(char*,char *ch);
int i,j;
void main()
{
/*token=strtok(str," ");
while(token!=NULL)
{
printf("%s\n",token);
token=strtok(NULL," ");
}*/
token=strtok1(str," ");
while(token!=NULL)
{
printf("%s\n",token);
token=strtok1(NULL," ");
}
}
char *strtok1(char*str,char *ch)
{
int k=j;
if(*str=='\0')
str=str+j;
for(i=j;str[i];i++)
{
if(str[i]==*ch)
{
str[i]='\0';
j=i+1;
break;
}
}
return str+k;
}
