#include"header.h"
void main()
{
char s[50];
char tab[]="\t";
printf("%d\n",sizeof(tab));
printf("enter the string:\n");
scanf("%[^\n]",s);
int i;
printf("%s\n",s);
printf("%d\n",strlen(s));
for(i=0;s[i];i++)
{
if(s[i]=='\t')
s[i]='\t';
}
printf("%s\n",s);
}
