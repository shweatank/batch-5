#include<stdio.h>
#include<string.h>
void main()
{
char str[20]="hello world bye";
char*token;
token=strtok(str," ");
while(token!=NULL)
{
printf("%s\n",token);
token=strtok(NULL," ");
}
printf("%s\n",str);
}
