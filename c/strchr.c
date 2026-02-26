#include<stdio.h>
void main()
{
char s[100],ch;
int i,len;
printf("Enter the string:\n");
scanf("%s",s);
printf("Enter the character:\n");
scanf(" %c",&ch);
for(i=0;s[i];i++)
{
if(s[i]==ch)
{
printf("Char is present\n");
return;
}
}
printf("char is not present\n");
}
